/* module NCproj_backspectmu */

/*
 general information: This module computes a weighted backprojection: the
 weight W(i,j) equals the amount of activity that has passed through the 
 voxel j and has contributed to detector pixel i.
 It uses precalculated indices and interpolation weights, stored in
 projcoeff (see NCprojcoeff).
*/

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : mar 1996
  Modifications :
* endtopic History

******************************************************************************/
#define __NCproj_backspectmu_module 1
#include "stdio.h"  /*NOINC*/
#include "NCproj_bproj.h"  /*NOINC*/
#include "math.h"  /*NOINC*/

/*enddeclars*/

/*-------------------------------------------------------------------------*/
/*global*/
void NCproj_backspectmu(
  float       *img,        /* the output image */
  float       *totweight,  /* receives the sum of weights in every pixel */
  float       *sinogram,   /* the input sinogram */
  float       *actimg,     /* the activity per pixel */
  float       *attenimg,   /* attenuation coeff per pixel */
  int          projnum,    /* current projection number */
  p_ProjCoeff  projcoeff   /* set of interpolation coefficients */
  )
 
  {
  p_ProjCoeff    prcf;
  int           det;  /* index of detector pixel in bigsino. */
  int           i;
  int          *i1;
  int          *i2;
  int           firstcommon, lastcommon;
  float        *w1;
  float        *w2;
  float        *imgplusoffset;
  float        *attplusoffset;
  float        *actplusoffset;
  float        *totweightplusoffset;
  float         proj;
  float         mu, selfmu;
  float         halfway;
  float         expminmu;
  static float  *globalmu = NULL;
  float         *p_mu;
  float          activ;


  if (globalmu == NULL) globalmu = (float *) calloc(1000, sizeof(float));
  prcf = projcoeff + projnum;
  NCproj_tobigsino(sinogram, projnum, projcoeff);

  for (det = 0; det < prcf->bigN; det++)
    {
    /* Pointers to indices and corresponding interpolation weights.
     * linefirst1 and linefirst2 are used to skip wat is out of the image.
     ***********/
    i1 = prcf->recindex1  + prcf->linefirst1[det];
    i2 = prcf->recindex2  + prcf->linefirst2[det];
    w1 = prcf->recweight1 + prcf->linefirst1[det];
    w2 = prcf->recweight2 + prcf->linefirst2[det];
    imgplusoffset       = img       + prcf->lineoffset[det];
    attplusoffset       = attenimg  + prcf->lineoffset[det];
    actplusoffset       = actimg    + prcf->lineoffset[det];
    totweightplusoffset = totweight + prcf->lineoffset[det];

    /* Determine the ranges in which 1) only point1, 2) only point2 and
     * 3) both points are available. Near the edges of the image, one of
     * the points required for interpolation may be outside the image, and
     * is then assumed to be zero.
     ****/
    if (prcf->linefirst1[det] < 0)
      {
      if (prcf->linefirst2[det] < 0)
        { /* no valid points */
        firstcommon = -1;
        lastcommon  = -1;
        }
      else
        { /* only point2 is valid */
        firstcommon = prcf->linelast2[det];
        lastcommon  = prcf->linelast2[det];
        }
      }
    else
      {
      if (prcf->linefirst2[det] < 0)
        { /* only point1 is valid */
        firstcommon = prcf->linelast1[det];
        lastcommon  = prcf->linelast1[det];
        }
      else
        { /* both points are valid for at least a part of the line */
        firstcommon = prcf->linefirst1[det];
        if (firstcommon < prcf->linefirst2[det])
          firstcommon = prcf->linefirst2[det];

        lastcommon = prcf->linelast1[det];
        if (lastcommon > prcf->linelast2[det])
          lastcommon = prcf->linelast2[det];
        }
      }

    proj    = prcf->bigsino[det];
    mu      = 0.0;
    /* 0.5 for half the pixel, 1/scale to take into account sampling distance
     * of 1 / scale along the projectino line: since there is only one pixel
     * per column (or per row), oblique lines are sampled with a step larger
     * than the pixel size of img and attenimg
     ***/
    halfway = 0.5 / prcf->scale;  

    /* Scanning the line and accumulate the overall attenuation in the
     * array globalmu (using the pointer p_mu).
     *****/
    p_mu = globalmu;
    if (prcf->linefirst1[det] >= 0)
      for (i = prcf->linefirst1[det]; i < firstcommon; i++)
        {
        selfmu  = *(attplusoffset + *i1++) * *w1++ * halfway;
        mu     += selfmu;
        *p_mu++ = exp(-mu);
        mu     += selfmu;
        }

    if (prcf->linefirst2[det] >= 0)
      for (i = prcf->linefirst2[det]; i < firstcommon; i++)
        {
        selfmu  = *(attplusoffset + *i2++) * *w2++ * halfway;
        mu     += selfmu;
        *p_mu++ = exp(-mu);
        mu     += selfmu;
        }

    for (i = firstcommon; i < lastcommon; i++)
      {
      selfmu  = (*(attplusoffset + *i1++) * *w1++ +
                 *(attplusoffset + *i2++) * *w2++) * halfway;
      mu     += selfmu;
      *p_mu++ = exp(-mu);
      mu     += selfmu;
      }

    if (prcf->linefirst1[det] >= 0)
      for (i = lastcommon; i < prcf->linelast1[det]; i++)
        {
        selfmu  = *(attplusoffset + *i1++) * *w1++ * halfway;
        mu     += selfmu;
        *p_mu++ = exp(-mu);
        mu     += selfmu;
        }

    if (prcf->linefirst2[det] >= 0)
      for (i = lastcommon; i < prcf->linelast2[det]; i++)
        {
        selfmu  = *(attplusoffset + *i2++) * *w2++ * halfway;
        mu     += selfmu;
        *p_mu++ = exp(-mu);
        mu     += selfmu;
        }

    /* Scan now in the opposite direction, accumulating the contribution from
     * all pixels to the attenuated projection. In every pixel, the value
     * accumulated so far is used as the backprojection weight. So every
     * pixel receives a weighted sinogram value, where the weight is equal to
     * the number of detected photons that are influenced by the attenuation
     * coefficient of that particular pixel.
     ***********************/
    p_mu--;
    i1--;
    i2--;
    w1--;
    w2--;
    activ = 0.0;

    if (prcf->linefirst2[det] >= 0)
      for (i = prcf->linelast2[det]-1; i >= lastcommon; i--)
        {
        activ += *(actplusoffset + *i2) * *w2 * *p_mu--;
        *(totweightplusoffset + *i2) +=        activ * *w2;
        *(imgplusoffset     + *i2--) += proj * activ * *w2--;
        }

    if (prcf->linefirst1[det] >= 0)
      for (i = prcf->linelast1[det]-1; i >= lastcommon; i--)
        {
        activ += *(actplusoffset + *i1) * *w1 * *p_mu--;
        *(totweightplusoffset + *i1) +=        activ * *w1;
        *(imgplusoffset     + *i1--) += proj * activ * *w1--;
        }

    for (i = lastcommon-1; i >= firstcommon; i--)
      {
      activ += (*(actplusoffset + *i1) * *w1 + 
                *(actplusoffset + *i2) * *w2) * *p_mu--;
      *(totweightplusoffset + *i2) +=        activ * *w2;
      *(totweightplusoffset + *i1) +=        activ * *w1;
      *(imgplusoffset     + *i2--) += proj * activ * *w2--;
      *(imgplusoffset     + *i1--) += proj * activ * *w1--;
      }

    if (prcf->linefirst2[det] >= 0)
      for (i = firstcommon-1; i >= prcf->linefirst2[det]; i--)
        {
        activ += *(actplusoffset + *i2) * *w2 * *p_mu--;
        *(totweightplusoffset + *i2) +=        activ * *w2;
        *(imgplusoffset     + *i2--) += proj * activ * *w2--;
        }

     if (prcf->linefirst1[det] >= 0)
      for (i < firstcommon-1; i >= prcf->linefirst1[det]; i--)
        {
        activ += *(actplusoffset + *i1) * *w1 * *p_mu--;
        *(totweightplusoffset + *i1) +=        activ * *w1;
        *(imgplusoffset     + *i1--) += proj * activ * *w1--;
        }
   }
  }




