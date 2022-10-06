/* module NCmlostr */

/*
 general information: 
 This module applies multiple iterations and subiterations of the
 ml-gradient ascent (sub)iteration.
 */

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : feb 1999.
  Modifications :
* endtopic History

******************************************************************************/
#define __NCmlostr_module 1
#include "stdio.h"  /*NOINC*/
#include "NCproj_bproj.h"  /*NOINC*/
#include "math.h"  /*NOINC*/

/*enddeclars*/

/*-------------------------------------------------------------------------*/
/*global*/
void NCmlostr(
  float        *recon,       /* reconstruction image */
  float        *sino,        /* sinogram */
  float        *blank,       /* attenuation sinogram or NULL */
  float        *contam,      /* contamination sinogram or NULL */
  int           nriter,      /* number of main iterations */
  int           nrsubsets,   /* number of subsets per iteration */
  float         relaxation,  /* relaxation coeff. Convergence certain if 1. */
  float       **likelihood,  /* receives pointer to float array with likelihood
                          values (nriter * nrsubsets values).
                          When nrsubsets > 1, likelihood is only approximate.
                          Ignored when NULL. */
  p_ProjCoeff   prcf,        /* precalc. coefficients: see NCprojcoeff */
  p_AbsPrior    absprior,    /* Absolute intensity prior or NULL */
  p_MarkovPrior markovprior, /* Markov prior or NULL */
  int           verbose)     /* When set, program prints info to screen */

  {
  float   *p_sino;
  float   *p_lik;
  float   *p_backatten;
  float    maxlikelihood;
  int      nrreconpixels;
  int     *subset;
  int     *subsetmask;
  int      previous;
  int      nrsinopixels;
  int      i;
  int      iter, subiter;
  int      subsetnum;


  /* Set negative values to zero in the sinogram.
   **********/
  nrsinopixels = prcf->N * prcf->nrproj;
  p_sino       = sino;
  for (i = 0; i < nrsinopixels; i++)
    {
    if (*p_sino < 0) *p_sino = 0.0;
    p_sino++;
    }

  /* Allocate array for the likelihood values if required.
   **********/
  if (likelihood != NULL)
    {
    *likelihood = (float *) calloc(nriter * nrsubsets, sizeof(float));
    p_lik = *likelihood;
    }
  else
    {
    p_lik = NULL;
    }

  /* Compute maximum likelihood value and normalize the likelihood values,
   * if required.
   **********/  
  if (likelihood != NULL)
    {
    if (verbose) printf("Computing upper limit of likelihood\n");
    maxlikelihood = 0.0;
    p_sino = sino;
    for (i = 0; i < nrsinopixels; i++)
      {
      if (*p_sino > VERYSMALL)
        {
        maxlikelihood += - *p_sino + *p_sino * log(*p_sino);
        }
      else
        {
        maxlikelihood += - VERYSMALL  + *p_sino * log(VERYSMALL);
        }
      p_sino++;
      }
    }

  /* Loop over iterations and subsets.
   **********/
  for (iter = 0; iter < nriter; iter++)
    {
    subsetnum = 0;
    for (subiter = 0; subiter < nrsubsets; subiter++)
      {
      subset = NCsino_subset(nrsubsets, prcf->nrproj, &subsetnum, &subsetmask,
                             &previous);
      NCmltrans(recon, sino, blank, contam, subset, relaxation, p_lik, 
                prcf, absprior, markovprior, verbose);
      free(subset);
      
      if (likelihood != NULL) 
        {
        *p_lik /= maxlikelihood;
        if (verbose) printf("Iter %d, subiter %d, likelihood = %g\n",
               iter, subiter, *p_lik);
        p_lik++;
        }
      }
    }



  }
