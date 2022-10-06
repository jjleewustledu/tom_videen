/* module NCproj_forward */

/*
 general information: This module computes the forward projection of
 an image along parallel projection lines. It uses precalculated
 indices and interpolation weights, stored in projcoeff (see
 NCprojcoeff).
*/

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : feb 1996
  Modifications :
* endtopic History

******************************************************************************/
#define __NCproj_forward_module 1
#include "NCproj_bproj.h"
#include "math.h"

/*enddeclars*/

/*-------------------------------------------------------------------------*/
/*global*/
void NCproj_forward(
  float       *img,
  float       *sinogram,
  int          projnum,
  p_ProjCoeff  projcoeff
  )
 
  {
  p_ProjCoeff   prcf;
  int           det;  /* index of detector pixel in bigsino. */
  int           i;
  int           einde;
  int          *i1;
  int          *i2;
  float        *w1;
  float        *w2;
  float        *imgplusoffset;
  float         proj;

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
    imgplusoffset = img + prcf->lineoffset[det];
    
    proj = 0.0;

    /* Scanning the line and summing the weighted values.
     *****/
    if (prcf->linefirst1[det] >= 0)
      {
      einde = prcf->linelast1[det];
      for (i = prcf->linefirst1[det]; i < einde; i++)
        {
        proj += *(imgplusoffset + *i1++) * *w1++;
        }
      }

    if (prcf->linefirst2[det] >= 0)
      {
      einde = prcf->linelast2[det];
      for (i = prcf->linefirst2[det]; i < einde; i++)
        {
        proj += *(imgplusoffset + *i2++) * *w2++;
        }
      }

    prcf->bigsino[det] = proj;
    }

  NCproj_frombigsino(sinogram, projnum, projcoeff);
  }




