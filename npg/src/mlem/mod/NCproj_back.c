/* module NCproj_back */

/*
 general information: 
 This module computes the backprojection of a sinogram line into an image.
 This routine is the transpose of NCproj_forward. The sinogram line must
 be available in the structure projcoeff (see NCproj_tobig and NCproj_frombig).
 */

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : feb 1996
  Modifications :
* endtopic History

******************************************************************************/
#define __NCproj_back_module 1
#include "NCproj_bproj.h"
#include "math.h"

/*enddeclars*/

/*-------------------------------------------------------------------------*/
/*global*/
void NCproj_back(
  float       *img,
  float       *sinogram,
  int          projnum,
  p_ProjCoeff  projcoeff
  )

  {
  p_ProjCoeff     prcf;
  int             det;  /* index of detector pixel in bigsino. */
  register int             *index;
  register float           *weight;
  register float           *imgplusoffset;
  register float           proj;
  register int             i;
  int             begin, einde;

  prcf = projcoeff + projnum;
  NCproj_tobigsino(sinogram, projnum, projcoeff);

  for (det = 0; det < prcf->bigN; det++)
    {
    /* Pointers to indices and corresponding interpolation weights.
     * linefirst1 and linefirst2 are used to skip wat is out of the image.
     ***********/
    
    proj = prcf->bigsino[det];

    if (proj != 0.0)
      {
      imgplusoffset = img + prcf->lineoffset[det];

      /* Scanning the line and filling in the weighted values.
       *****/
      begin = prcf->linefirst1[det];
      if (begin >= 0)
        {
        index  = prcf->recindex1  + begin;
        weight = prcf->recweight1 + begin;
        einde  = prcf->linelast1[det];
        for (i = begin; i < einde; i++)
          {
          *(imgplusoffset + *index++) += proj * *weight++;
          }
        }

      begin = prcf->linefirst2[det];
      if (begin >= 0)
        {
        index  = prcf->recindex2  + begin;
        weight = prcf->recweight2 + begin;
        einde  = prcf->linelast2[det];
        for (i = begin; i < einde; i++)
          {
          *(imgplusoffset + *index++) += proj * *weight++;
          }
        }
      }
    }
 }
