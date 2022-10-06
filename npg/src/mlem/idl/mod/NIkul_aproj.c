/* module NIkul_aproj */

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
 *        float       *img
 *        float       *sinogram
 *        float       *attenimg
 *        int          projnum
 *        p_ProjCoeff  prcf    (a pointer: long int in IDL)
 */
int NIkul_aproj (
                  int   argc,
                  void *argv[]
                  )
 
  {
  p_ProjCoeff   prcf;
  float        *img;
  float        *sinogram;
  float        *attenimg;
  int           projnum;
  
  /*
   * Count the number of arguments
   */
  if (argc != 5)
    {
    printf("NIkul_aproj: 5 arguments required, %d supplied\n", argc);
    return(0);
    }

  img        = (float *) argv[0];
  sinogram   = (float *) argv[1];
  attenimg   = (float *) argv[2];
  projnum    = (int)         (* (short *) argv[3]);
  prcf       = (p_ProjCoeff) (* (int *) argv[4]);

  NCproj_attforward(img, sinogram, attenimg, projnum, prcf);

  return(1);
  }

