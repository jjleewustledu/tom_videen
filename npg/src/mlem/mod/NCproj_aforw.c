/* module NCproj_attforward */

/*
 general information: This module computes the forward attenuated
 projection of an image along parallel projection lines. It uses
 precalculated indices and interpolation weights, stored in projcoeff
 (see NCprojcoeff).
*/

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : mar 1996
  Modifications :
* endtopic History

******************************************************************************/
#define __NCproj_attforward_module 1
#include "NCproj_bproj.h"
#include "math.h"

/*enddeclars*/

/*-------------------------------------------------------------------------*/
/*global*/
void NCproj_attforward(
  float       *img,        /* the input image */
  float       *sinogram,   /* the output sinogram */
  float       *attenimg,   /* attenuation coeff per pixel */
  int          projnum,    /* current projection number */
  p_ProjCoeff  projcoeff   /* set of interpolation coefficients */
  )
 
  {
  p_ProjCoeff   prcf;
  int           det;  /* index of detector pixel in bigsino. */
  int           i;
  int          *i1;
  int          *i2;
  int           firstcommon, lastcommon;
  float        *w1;
  float        *w2;
  float        *imgplusoffset;
  float        *attplusoffset;
  float         proj;
  float         mu, selfmu;
  float         halfway;

  prcf = projcoeff + projnum;

  for (det = 0; det < prcf->bigN; det++)
    {
    /* Pointers to indices and corresponding interpolation weights.
     * linefirst1 and linefirst2 are used to skip wat is out of the image.
     ***********/
    i1 = prcf->recindex1  + prcf->linefirst1[det];
    i2 = prcf->recindex2  + prcf->linefirst2[det];
    w1 = prcf->recweight1 + prcf->linefirst1[det];
    w2 = prcf->recweight2 + prcf->linefirst2[det];
    imgplusoffset = img      + prcf->lineoffset[det];
    attplusoffset = attenimg + prcf->lineoffset[det];
    
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

    proj    = 0.0;
    mu      = 0.0;
    /* 0.5 for half the pixel, 1/scale to take into account sampling distance
     * of 1 / scale along the projectino line: since there is only one pixel
     * per column (or per row), oblique lines are sampled with a step larger
     * than the pixel size of img and attenimg
     ***/
    halfway = 0.5 / prcf->scale;  

    /* Scanning the line and summing the weighted values.
     *****/
    if (prcf->linefirst1[det] >= 0)
      for (i = prcf->linefirst1[det]; i < firstcommon; i++)
        {
        selfmu  = *(attplusoffset + *i1) * *w1 * halfway;
        mu     += selfmu;
        proj   += *(imgplusoffset + *i1++) * *w1++ * exp(-mu);
        mu     += selfmu;
        }

    if (prcf->linefirst2[det] >= 0)
      for (i = prcf->linefirst2[det]; i < firstcommon; i++)
        {
        selfmu  = *(attplusoffset + *i2) * *w2 * halfway;
        mu     += selfmu;
        proj   += *(imgplusoffset + *i2++) * *w2++ * exp(-mu);
        mu     += selfmu;
        }

    for (i = firstcommon; i < lastcommon; i++)
      {
      selfmu  = (*(attplusoffset + *i1) * *w1 +
                 *(attplusoffset + *i2) * *w2) * halfway;
      mu     += selfmu;
      proj   += (*(imgplusoffset + *i1++) * *w1++ + 
                 *(imgplusoffset + *i2++) * *w2++) * exp(-mu);
      mu     += selfmu;
      }

    if (prcf->linefirst1[det] >= 0)
      for (i = lastcommon; i < prcf->linelast1[det]; i++)
        {
        selfmu  = *(attplusoffset + *i1) * *w1 * halfway;
        mu     += selfmu;
        proj   += *(imgplusoffset + *i1++) * *w1++ * exp(-mu);
        mu     += selfmu;
        }

    if (prcf->linefirst2[det] >= 0)
      for (i = lastcommon; i < prcf->linelast2[det]; i++)
        {
        selfmu  = *(attplusoffset + *i2) * *w2 * halfway;
        mu     += selfmu;
        proj   += *(imgplusoffset + *i2++) * *w2++ * exp(-mu);
        mu     += selfmu;
        }

    prcf->bigsino[det] = proj;
    }

  NCproj_frombigsino(sinogram, projnum, projcoeff);
  }




