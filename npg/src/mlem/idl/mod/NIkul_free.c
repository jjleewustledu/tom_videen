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

/*---------------------------------------------------------------------------*/
/*
 * Expects:
 *    long  prcf        a pointer to the structure with the coefficients.
 */
int NIkul_free (
                  int   argc,
                  void *argv[]
                  )
 
  {
  p_ProjCoeff                prcf;

  /*
   * Count the number of arguments
   */
  if (argc != 1)
    {
    printf("NIkul_free: 1 argument required, %d supplied\n", argc);
    return(0);
    }

  prcf       = (p_ProjCoeff)    (* (int *) argv[0]);
  NCproj_free(&prcf);
  return((int) prcf);
  }

