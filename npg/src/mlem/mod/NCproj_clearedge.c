/* module NCproj_clearedge */

/*
 general information: 
 This module clears the first and last point of each projection line, to
 suppress edge effects (numerical instability if a pixel has smaller effect
 in forward projection than in backprojection).
 */

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : feb 1996
  Modifications :
* endtopic History

******************************************************************************/
#define __NCproj_clearedge_module 1
#include "NCproj_bproj.h"
#include "math.h"

/*enddeclars*/

/*-------------------------------------------------------------------------*/
/*global*/
void NCproj_clearedge(
  float       *img,
  int          projnum,
  p_ProjCoeff  projcoeff
  )

  {
  p_ProjCoeff   prcf;
  int           det;  /* index of detector pixel in bigsino. */
  int          *i1;
  int          *i2;
  int          *e1;
  int          *e2;
  float        *imgplusoffset;

  prcf = projcoeff + projnum;

  for (det = 0; det < prcf->bigN; det++)
    {
    /* Pointers to indices and corresponding interpolation weights.
     * linefirst1 and linefirst2 are used to skip wat is out of the image.
     ***********/
    i1 = prcf->recindex1  + prcf->linefirst1[det];
    i2 = prcf->recindex2  + prcf->linefirst2[det];
    e1 = prcf->recindex1 +  prcf->linelast1[det]-1;
    e2 = prcf->recindex2 +  prcf->linelast2[det]-1;
    imgplusoffset = img + prcf->lineoffset[det];
    if (prcf->linefirst1[det] >= 0)
      {
      *(imgplusoffset + *i1) = 0.0;
      *(imgplusoffset + *e1) = 0.0;
      if (i1 < e1)
        {
        *(imgplusoffset + *(i1+1)) = 0.0;
        *(imgplusoffset + *(e1-1)) = 0.0;
        }
      }
    if (prcf->linefirst2[det] >= 0)
      {
      *(imgplusoffset + *i2) = 0.0;
      *(imgplusoffset + *e2) = 0.0;
      if (i2 < e2)
        {
        *(imgplusoffset + *(i2+1)) = 0.0;
        *(imgplusoffset + *(e2-1)) = 0.0;
        }
      }
    }
 }
