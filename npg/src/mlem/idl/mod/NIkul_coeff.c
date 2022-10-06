/* module NIkul_coeff */

/*
Calculates a matrix of projection weights
*/

/*******************************************************************************

* topic History
    Author(s)     : J. Nuyts
    Date          : april 1996
    Modifications : 
* endtopic History


*******************************************************************************/
#include "NCproj_bproj.h"   /*NOINC*/
#include "math.h"

/*---------------------------------------------------------------------------*/
/*
 * Expects:
 *    int   nrdet,      number of detectors = number of cols, rows in image
 *    int   nrproj,     number of projections in sinogram.
 *    float startangle, starting angle, in radians
 *    float deltaangle, offset to next projection, in radians
 *    float radius      radius of the FOV in pixels. Use -1 for the entire
 *                      image. Use nrdet/2 to reconstruct the area that
 *                      has contributed to EVERY projection.
 *    returns an integer containing a pointer.
 */
int NIkul_coeff (
                  int   argc,
                  void *argv[]
                  )
 
  {
  p_ProjCoeff                prcf;
  int                        nrdet;  
  int                        nrproj;
  float                      startangle;
  float                      deltaangle;
  float                      radius;

  
  /*
   * Count the number of arguments
   */
  if (argc != 5)
    {
    printf("NIkul_coeff: 6 arguments required, %d supplied\n", argc);
    return(0);
    }

  nrdet      = (int)    (* (short *) argv[0]);
  nrproj     = (int)    (* (short *) argv[1]);
  startangle = (float)  (* (float *) argv[2]);
  deltaangle = (float)  (* (float *) argv[3]);
  radius     = (float)  (* (float *) argv[4]);

  prcf = NCprojcoeff(nrdet, nrproj, startangle, deltaangle, radius);

  return( (int) prcf );
  }

