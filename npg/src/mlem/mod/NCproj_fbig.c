/* module NCproj_frombig */

/*
 general information: 
 This module interpolates a sinogram line from an increased number of pixels
 into the regular number of pixels, corresponding to an increase in pixel
 size.
 This operation is the opposite of NCproj_tobig. It uses predefined
 coefficients, which are stored in projcoeff by the routine NCprojcoeff.
 The array containing the oversampled sinogram is in the same structure.
 */

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : feb 1996
  Modifications :
* endtopic History

******************************************************************************/
#define __NCproj_frombig_module 1
#include "NCproj_bproj.h"
#include "math.h"

/*enddeclars*/

/*-------------------------------------------------------------------------*/
/*global*/
void NCproj_frombigsino(
  float       *sinogram,
  int          projnum,
  p_ProjCoeff  projcoeff
  )

  {
  p_ProjCoeff   prcf;
  float        *sino;
  float        *bigsino;
  int           i;
  int          *i1;
  int          *i2;
  float        *w1;
  float        *w2;


  prcf    = projcoeff + projnum;           /* precalc data for this angle */
  sino    = sinogram + projnum * prcf->N;  /* sinogram line for this angle */
  bigsino = prcf->bigsino;
  i1      = prcf->bigindex1;
  i2      = prcf->bigindex2;
  w1      = prcf->bigweight1;
  w2      = prcf->bigweight2;

  for (i = 0; i < prcf->N; i++)
    {
    *sino++ = *(bigsino + *i1++) * *w1++ + *(bigsino + *i2++) * *w2++;
    }  
  }


