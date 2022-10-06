/* module NCmarkov */

/*
 general information: 
 This module computes the gradient of a Gibbs prior (Markov Random Field).
 The clique function is quadratic, Huber or Geman.
 Neighborhood is 4 or 8.
 */

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : feb 1999.
  Modifications :
* endtopic History

******************************************************************************/
#define __NCmarkov_module 1
#include "stdio.h"  /*NOINC*/
#include "NCproj_bproj.h"  /*NOINC*/
#include "math.h"  /*NOINC*/

/*enddeclars*/

/*-------------------------------------------------------------------------*/
#define MACRO_QUAD(result, center, neighbor, delta, deltaquad, weight) \
  result += weight * ((neighbor) - (center)) / deltaquad

#define MACRO_HUBER(result, center, neighbor, delta, deltaquad, weight) \
  difference = (neighbor) - (center); \
  if ((difference * difference) < deltaquad) \
    result += weight * difference / deltaquad; \
  else \
    if (difference >= 0) \
      result += weight / delta; \
    else \
      result += - weight / delta

#define MACRO_GEMAN(result, center, neighbor, delta, deltaquad, weight) \
  difference = (neighbor) - (center); \
  dummy = (difference * difference + 2.0 * deltaquad); \
  result += weight * 4.0 * difference * deltaquad / dummy / dummy;

/*-------------------------------------------------------------------------*/
/*global*/
void NCmarkov(
  float    *gprior,       /* Image receiving first derivative of the prior */
  float    *g2prior,      /* Receives second derivative: Single value! */
  float    *img,          /* The input image */
  int       ncols,        /* Number of columns in the input image */
  int       nrows,        /* Number of rws in the input image */
  float     delta,        /* Parameter (standard deviation or similar) for the
                             energy function */
  int       neighborhood, /* 4 or 8 neighborhood */
  char      functiontype) /* 'q' = quadratic, 'h' = Huber, 'g' = Geman */

  {
  float   deltaquad;
  float   dummy;
  float   difference;
  float  *left;
  float  *p_img;
  float  *gleft;
  float  *p_gprior;
  float   weight;
  float   weight4, weight8;
  int     col;
  int     row;
  int     num;
  int     offset;
  int     lastcol, lastrow;

  /* Set entire image to zero.
  ********/
  p_gprior = gprior;
  for (num = 0; num < ncols*nrows; num++)
    *p_gprior++ = 0.0;

  lastcol = ncols-2; /* to avoid image border */
  lastrow = nrows-2;

  if (neighborhood == 4)
    {
    weight4 = 0.25;  /* 1/4 */
    }
  else
    {
    weight4 = 0.146447;  /* 1 / (4 + 4 / 2^0.5) */
    weight8 = 0.103553;  /* 1/2^0.5  / (4 + 4 / 2^0.5) */
    }

  deltaquad = delta * delta;
  *g2prior = 1.0 / deltaquad;
  /* scan all possible neighbors (4 or 8) */
  for (num = 0; num < neighborhood; num++)
    {
    switch (num)
      {
      case 0: offset = -1;         /* left */
              weight = weight4;
              break;
      case 1: offset =  1;         /* right */
              weight = weight4;
              break;
      case 2: offset = -ncols;     /* up */
              weight = weight4;
              break;
      case 3: offset =  ncols;     /* down */
              weight = weight4;
              break;
      case 4: offset = -1 - ncols; /* up left */
              weight = weight8;
              break;
      case 5: offset =  1 - ncols; /* up right */
              weight = weight8;
              break;
      case 6: offset = -1 + ncols; /* down left */
              weight = weight8;
              break;
      case 7: offset =  1 + ncols; /* down right */
              weight = weight8;
              break;
      }

    left  = img    + 1 + ncols;  /* skip first row and first column */
    gleft = gprior + 1 + ncols;

    switch(functiontype)
      {
      case 'q' : 
        for (row = 1; row <= lastrow; row++) /* avoid edge pixel */
          {
          p_img    = left;
          p_gprior = gleft;
          for (col = 1; col <= lastcol; col++)
            {
            MACRO_QUAD(*p_gprior, *p_img, *(p_img+offset), delta, deltaquad,
                       weight);
            p_gprior++;
            p_img++;
            }
          left  += ncols;
          gleft += ncols;
          }
        break;

      case 'h' : 
        for (row = 1; row <= lastrow; row++) /* avoid edge pixel */
          {
          p_img    = left;
          p_gprior = gleft;
          for (col = 1; col <= lastcol; col++)
            {
            MACRO_HUBER(*p_gprior, *p_img, *(p_img+offset), delta, deltaquad,
                       weight);
            p_gprior++;
            p_img++;
            }
          left  += ncols;
          gleft += ncols;
          }
        break;

      case 'g' :
        for (row = 1; row <= lastrow; row++) /* avoid edge pixel */
          {
          p_img    = left;
          p_gprior = gleft;
          for (col = 1; col <= lastcol; col++)
            {
            MACRO_GEMAN(*p_gprior, *p_img, *(p_img+offset), delta, deltaquad,
                       weight);
            p_gprior++;
            p_img++;
            }
          left  += ncols;
          gleft += ncols;
          }
        break;
      }
    }
  }

