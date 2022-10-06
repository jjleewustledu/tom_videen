/* module NIkul_coeff2 */

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
 *    float *coroffset  Center of Rotation offset. When set to NULL pointer,
 *                      ignored.
 *    float  arc_radius Radius in pixels, used for arccorrection. Ignored when
 *                      zero or negative.
 *    returns an integer containing a pointer.
 */
int NIkul_coeff3 (
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
  float                     *coroffset;
  float                      arc_radius;

  
  /*
   * Count the number of arguments
   */
  if (argc != 7)
    {
    printf("NIkul_coeff2: 7 arguments required, %d supplied\n", argc);
    return(0);
    }

  nrdet      = (int)    (* (short *) argv[0]);
  nrproj     = (int)    (* (short *) argv[1]);
  startangle = (float)  (* (float *) argv[2]);
  deltaangle = (float)  (* (float *) argv[3]);
  radius     = (float)  (* (float *) argv[4]);
  coroffset  = (float *) argv[5];
  arc_radius = (float)  (* (float *) argv[6]);

  prcf = NCprojcoeff3(nrdet, nrproj, startangle, deltaangle, radius, 
                      coroffset, arc_radius);

  return( (int) prcf );
  }

