/* module NCextend_nrm */

/*
 general information: 
 This module computes the sinograms row belonging to the next subset.
 */

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : mar 1996
  Modifications :
* endtopic History

******************************************************************************/
#define __NCextend_nrm_module 1
#include "NCproj_bproj.h"
#include "math.h"

/*enddeclars*/

/*-------------------------------------------------------------------------*/
/*global*/
float NCextend_nrm(
                  float  *nrmout,  /* 2D output normalization sinogram */
                  float  *nrmin,   /* 2D input normalization sinogram */
                  int     nrdet,   /* number of detectors = nr of columns */
                  int     nrproj,  /* number of projections = nr of rows */
                  float   toosmall) /* Values < toosmall are set to the mean
                                       of the higher values */
  {
  float   *p_nrmout;
  float   *p_nrmin;
  float    meanval;
  int      nrpixels;
  int      totpixels;
  int      i;
  
  totpixels = nrdet * nrproj;

  /* Find the mean of all sufficiently high pixel values.
   **********/
  meanval   = 0.0;
  nrpixels  = 0;
  p_nrmin = nrmin;
  for (i = 0; i < totpixels; i++)
    {
    if (*p_nrmin > toosmall)
      {
      meanval += *p_nrmin;
      nrpixels++;
      }
    p_nrmin++;
    }
  meanval /= nrpixels;

  /* Replace all zero (or small) values with that mean value.
   **********/
  p_nrmin  = nrmin;
  p_nrmout = nrmout;
  for (i = 0; i < totpixels; i++)
    {
    if (*p_nrmin > toosmall)
      {
      *p_nrmout = *p_nrmin;
      }
    else
      {
      *p_nrmout = meanval;
      }
    p_nrmin++;
    p_nrmout++;
    }
 
  return(meanval);
  }

