/* module NCproj_maxim */

/*
 general information: This module computes the forward maximum intensity
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
#define __NCproj_maxim_module 1
#include "NCproj_bproj.h"
#include "math.h"

/*enddeclars*/

/*-------------------------------------------------------------------------*/
/*global*/
void NCproj_maxim(
  float       *img,        /* the input image */
  float       *sinogram,   /* the output sinogram */
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
  float         maxproj;
  float         proj;
  float        *sino;
  float        *bigsino;

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

    maxproj = 0.0;
    proj    = 0.0;

    /* Scanning the line and summing the weighted values.
     *****/
    if (prcf->linefirst1[det] >= 0)
      for (i = prcf->linefirst1[det]; i < firstcommon; i++)
        {
        proj   = *(imgplusoffset + *i1++) * *w1++;
        if (proj > maxproj) maxproj = proj;
        }

    if (prcf->linefirst2[det] >= 0)
      for (i = prcf->linefirst2[det]; i < firstcommon; i++)
        {
        proj   = *(imgplusoffset + *i2++) * *w2++;
        if (proj > maxproj) maxproj = proj;
        }

    for (i = firstcommon; i < lastcommon; i++)
      {
      proj    = (*(imgplusoffset + *i1++) * *w1++ + 
                 *(imgplusoffset + *i2++) * *w2++);
      if (proj > maxproj) maxproj = proj;
      }

    if (prcf->linefirst1[det] >= 0)
      for (i = lastcommon; i < prcf->linelast1[det]; i++)
        {
        proj   = *(imgplusoffset + *i1++) * *w1++;
        if (proj > maxproj) maxproj = proj;
        }

    if (prcf->linefirst2[det] >= 0)
      for (i = lastcommon; i < prcf->linelast2[det]; i++)
        {
        proj    = *(imgplusoffset + *i2++) * *w2++;
        if (proj > maxproj) maxproj = proj;
        }

    prcf->bigsino[det] = maxproj;
    }

  sino    = sinogram + projnum * prcf->N;  /* sinogram line for this angle */
  bigsino = prcf->bigsino;
  i1      = prcf->bigindex1;
  i2      = prcf->bigindex2;
  w1      = prcf->bigweight1;
  w2      = prcf->bigweight2;

  for (i = 0; i < prcf->N; i++)
    {
    *sino++ = (*(bigsino + *i1++) * *w1 + *(bigsino + *i2++) * *w2)
              / (*w1 + *w2);
    w1++;
    w2++;
    }  
  }




