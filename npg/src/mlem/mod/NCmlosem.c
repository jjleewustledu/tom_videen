/* module NCmlosem */

/*
 general information: 
 This module computes the sinograms row belonging to the next subset.
 */

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : mar 1996
  Modifications : 11-sep-1997: backprojection of attenuation was computed
        for all projections, and then reused for all subsets.
        We found that convergence is faster if the backprojection of the
        attenuation is computed for the subset only. It requires more
        computation, but convergence is faster.
   
* endtopic History

******************************************************************************/
#define __NCmlosem_module 1
#include "stdio.h"         /*NOINC*/
#include "NCproj_bproj.h"  /*NOINC*/
#include "math.h"          /*NOINC*/

/*enddeclars*/

/*-------------------------------------------------------------------------*/
/*global*/
void NCmlosem(
  float        *recon,  /* start image, receives final image */
  float        *sino,   /* measured sinogram: neg. values are set to 0 !!! */
  float        *atten,  /* attenuation or NULL if not available */
  int           nriter, /* number of main iterations */
  int           nrsubsets, /* number of subsets per iteration */
  float       **likelihood, /* receives pointer to float array with likelihood
                          values (nriter * nrsubsets values).
                          When nrsubsets > 1, likelihood is only approximate.
                          Ignored when NULL. */
  p_ProjCoeff   prcf,       /* precalc. coefficients: see NCprojcoeff */
  int           verbose)  /* When set, program prints info to screen */

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
  int      i, j;
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

  /* If attenuation is available, precalculate the backprojection of the
   * attenuation coefficients over all projections.
   *    * J. Nuyts 11-sep-1997: This is now only done if
   * there is only one subset. For multiple subsets, the backprojection of
   * the attenuation may be recomputed for each subset in NCml. Convergence is
   * faster if the backprojection of the attenuation is restricted to the
   * current subset (and therefor recomputed), as compared to computing it
   * just once using all projections, but calculation time slows down.
   * Currently: precalculation turned OFF: 
   * when turned on: slight dip in the center of a cylinder when subsets
   * are being used (even when decreasing nrsub!).
   **********/
  if ((atten != NULL) && (! prcf->backatten_ok) && (nrsubsets == 1))
    {
    if (verbose) printf("Backprojection of the attenuation coefficients...\n");
    p_backatten = prcf->backatten;
    nrreconpixels = prcf->N * prcf->N;
    for (i = 0; i < nrreconpixels; i++)
      {
      *p_backatten++ = 0;
      }
    for (i = 0; i < prcf->nrproj; i++)
      {
      NCproj_back(prcf->backatten, atten, i, prcf);
      }

    /*
    for (i = 0; i < nrreconpixels; i++)
      {
      *p_backatten++ /= prcf->nrproj;
      }    
    for (i = 0; i < prcf->nrproj; i++)
      {
      NCproj_clrfov(prcf->backatten, prcf->N, prcf->N, 1.0);
      }
      */

    p_backatten = prcf->backatten;
    for (i = 0; i < nrreconpixels; i++)
      {
      if (*p_backatten > 1e-6)
        *p_backatten /= prcf->nrproj;
      else
        *p_backatten = 1.;
      p_backatten++;
      }

    prcf->backatten_ok = 1;
    if (verbose) printf("done.\n");
    }
  else prcf->backatten_ok = prcf->backatten_ok && (nrsubsets == 1);

  /* Loop over iterations and subsets.
   **********/
  for (iter = 0; iter < nriter; iter++)
    {
    subsetnum = 0;
    for (subiter = 0; subiter < nrsubsets; subiter++)
      {
      subset = NCsino_subset(nrsubsets, prcf->nrproj, &subsetnum, &subsetmask,
                             &previous);
      NCml(recon, sino, atten, subset, p_lik, prcf);
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
