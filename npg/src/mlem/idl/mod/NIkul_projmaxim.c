/* module NIkul_projmaxim */

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
 *        int          projnum
 *        p_ProjCoeff  prcf    (a pointer: long int in IDL)
 */
int NIkul_projmaxim (
                  int   argc,
                  void *argv[]
                  )
 
  {
  p_ProjCoeff   prcf;
  float        *img;
  float        *sinogram;
  int           projnum;
  
  /*
   * Count the number of arguments
   */
  if (argc != 4)
    {
    printf("NIkul_projmaxim: 4 arguments required, %d supplied\n", argc);
    return(0);
    }

  img        = (float *) argv[0];
  sinogram   = (float *) argv[1];
  projnum    = (int)         (* (short *) argv[2]);
  prcf       = (p_ProjCoeff) (* (int *) argv[3]);

  NCproj_maxim(img, sinogram, projnum, prcf);

  return(1);
  }

