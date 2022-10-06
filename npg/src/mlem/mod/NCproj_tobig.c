/* module NCproj_tobig */

/*
 general information: 
 This module interpolates a sinogram line, increasing the number of pixels
 (and therefore, decreasing the pixel size). It uses the predefined 
 interpolation coefficients and indices, and stores the result in a
 pre-allocated array.
 All the predefined and pre-allocated stuff is in projcoeff, which is
 produced by NCprojcoeff.c
 */

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : feb 1996
  Modifications :
* endtopic History

******************************************************************************/
#define __NCproj_tobig_module 1
#include "NCproj_bproj.h"
#include "math.h"

/*enddeclars*/

/*-------------------------------------------------------------------------*/
/*global*/
void NCproj_tobigsino(
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
  i1      = prcf->smallindex1;
  i2      = prcf->smallindex2;
  w1      = prcf->smallweight1;
  w2      = prcf->smallweight2;
  
  for (i = 0; i < prcf->bigN; i++)
    {
    *bigsino++ = *(sino + *i1++) * *w1++ + *(sino + *i2++) * *w2++;
    }
  }

