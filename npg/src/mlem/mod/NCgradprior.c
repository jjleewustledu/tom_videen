/* module NCgradprior */

/*
 general information: 
 This module computes the gradient of an absolute intensity prior.
 */

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : feb 1999.
  Modifications :
* endtopic History

******************************************************************************/
#define __NCgradprior_module 1
#include "stdio.h"  /*NOINC*/
#include "NCproj_bproj.h"  /*NOINC*/
#include "math.h"  /*NOINC*/

/*enddeclars*/

/*-------------------------------------------------------------------------*/
/*global*/
void NCgradprior(
  float    *gprior,     /* Gets the first derivative of the prior */
  float    *g2prior,    /* Gets estimate the second derivative of the prior */
  float    *img,        /* The input image */
  int       nrpixels,   /* Total number of pixels in the input image */
  int       nrmodes,    /* Number of modes of the prior distribution */
  float    *priormean,  /* Mean of each mode, sorted from low to high */
  float    *priorstd,   /* Standard deviation: detemines only the strength */
  float    *priorwidth) /* The width of each mode: range of the mode */

  {
  float  *p_gprior;
  float  *p_g2prior;
  float  *p_img;
  float  *p_mean;
  float  *p_width;
  float  *std2m1;
  float  *p_std2m1;
  float  *corner;
  float  *slope;
  float  *p_corner;
  float  *p_slope;
  float  *center;
  float  *p_center;
  float   lastmean;
  float   laststd2;
  int     nrcorner;
  int     offset;
  int     pixel;
  int     mode;
  int     i;
  int     soort;

  /* We need 1 devided by the square of the standard deviation
  *******/
  std2m1   = (float *) calloc(nrmodes, sizeof(float));
  for (mode = 0; mode < nrmodes; mode++)
    std2m1[mode] = 1. / (priorstd[mode] * priorstd[mode]);

  offset      = nrmodes - 1;
  lastmean    = *(priormean + offset);
  laststd2    = *(std2m1 + offset);


  /* For every mode, we need to define its range. We do that by
   * defining additional points in between the prior-means. The position
   * is determined by the value priorwidth. Let us call those A-points.
   * There are (nrmodes - 1) A-points, separating the maxima of the prior.
   * In the middle between an A-point and a prior-mean, we need an
   * additional point, which we will call a B-point. There are (2
   * nrmodes - 2) B-points.
   * The piecewise linear gradient changes slope in the
   * A-points and in the B-points.
   * In total, we have 3*(nrmodes-1) points. We need to add an extra
   * point at the end.
   * All this is irrelevant if there is only one mode.
   *********/
  if (nrmodes > 1)
    {
    nrcorner = 3 * nrmodes - 2;
    corner = (float *) calloc(nrcorner, sizeof(float));
    slope  = (float *) calloc(nrcorner, sizeof(float));
    center = (float *) calloc(nrcorner, sizeof(float));
    
    /* Now we define the slope for each corner point the slope that
     * we want at the left hand side of that point.
     *******/
    p_std2m1 = std2m1;
    p_corner = corner;
    p_center = center;
    p_slope  = slope;
    p_mean   = priormean;
    p_width  = priorwidth;
    soort = 1;
    for (i = 0; i < nrmodes-1; i++)
      {
      *(p_corner + 1) = (*p_mean * *(p_width+1) +  *(p_mean+1) * *p_width) /
                              (*p_width + *(p_width+1));
      *p_corner       = (*p_mean       + *(p_corner + 1)) / 2.0;
      *(p_corner + 2) = (*(p_mean + 1) + *(p_corner + 1)) / 2.0;
      
      *p_slope       =   *p_std2m1;
      *(p_slope + 1) = - *p_std2m1;
      *(p_slope + 2) = - *(p_std2m1+1);
      
      *p_center       = *p_mean;
      *(p_center + 1) = *(p_corner + 1);
      *(p_center + 2) = *(p_corner + 1);
      
      p_corner += 3;
      p_center += 3;
      p_slope  += 3;
      p_mean++;
      p_width++;
      p_std2m1++;
      }
    *p_corner = lastmean;
    *p_center = lastmean;
    *p_slope  = laststd2;
    
    /* for (i = 0; i < nrcorner; i++)
       printf(";%g, %g, %g\n", corner[i], center[i], slope[i]); */
    }


  /* Run over all pixels
  ********/
  p_img     = img;
  p_gprior  = gprior;
  p_g2prior = g2prior;
  for (pixel = 0; pixel < nrpixels; pixel++)
    {
    /* Value is smaller than lowest mean
    *********/
    if (*p_img <= *priormean)
      {
      *p_gprior++  = (*priormean - *p_img++) * *std2m1;
      *p_g2prior++ = *std2m1;
      }

    /* Value is higher than lowest mean
    *********/
    else if (*p_img >= lastmean)
      {
      *p_gprior++ = (lastmean - *p_img++) * laststd2;
      *p_g2prior++ = laststd2;
      }

    /* Value somewhere between two means
    *********/
    else
      {
      p_corner = corner;
      while (*p_corner < *p_img) p_corner++;
      offset = (int) (p_corner - corner);
      *p_g2prior = *(slope + offset);
      *p_gprior++ = (*(center + offset) - *p_img++) * *p_g2prior;
      if (*p_g2prior < 0) *p_g2prior *= -1.0;
      p_g2prior++;
      }
    }

  free(std2m1);
  free(corner);
  free(slope);
  free(center);
  }



