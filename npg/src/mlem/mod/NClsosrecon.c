/* module NClsosrecon */

/*
 general information: 
 This module computes the sinograms row belonging to the next subset.
 */

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : mei 1998
  Modifications : 
   
* endtopic History

******************************************************************************/
#define __NClsosrecon_module 1
#include "stdio.h"         /*NOINC*/
#include "NCproj_bproj.h"  /*NOINC*/
#include "math.h"          /*NOINC*/

/*enddeclars*/

/*-------------------------------------------------------------------------*/
/*global*/
void NClsosrecon(
  float        *recon,    /* start image, receives final image */
  float        *sino,     /* measured sinogram */
  float        *variance,   /* sinogram with variance values. Should be
                              strictly positive, because in denominator */
  float        *atten,  /* attenuation or NULL if not available */
  int           nriter, /* number of main iterations */
  int           nrsubsets, /* number of subsets per iteration */
  float         relaxation, /* relaxation value. In the order of 1..2 */
  float       **likelihood, /* receives pointer to float array with likelihood
                          values (nriter * nrsubsets values).
                          When nrsubsets > 1, likelihood is only approximate.
                          Ignored when NULL. */
  int         emission,    /* When set, mimics MLEM for large pixels */
  float     **backmlem,    /* Only used when emission is set. Allocated
                              internally if necessary. */
  p_ProjCoeff   prcf,       /* precalc. coefficients: see NCprojcoeff */
  int           verbose)  /* When set, program tells a lot */

  {
  float   *p_sino;
  float   *p_varsino;
  float   *p_lik;
  float   *p_backatten;
  float   *p_backmlem;
  float   *p_atten;
  float   *p_calcsino;
  float    maxlikelihood;
  float   *p_worksino;
  int      nrreconpixels;
  int      nrsinopixels;
  int     *subset;
  int     *subsetmask;
  int      previous;
  int      i, j;
  int      iter, subiter;
  int      subsetnum;



  nrsinopixels = prcf->N * prcf->nrproj;
  nrreconpixels = prcf->N * prcf->N;
  
  /* Set negative values to zero in the sinogram.
   **********/
  if (emission)
    {
    p_sino       = sino;
    for (i = 0; i < nrsinopixels; i++)
      {
      if (*p_sino < 0) *p_sino = 0.0;
      p_sino++;
      }
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
    for (j = 0; j < 2; j++)
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
    }

  /* ML-EM normalization if emission is set, and if an attenuation sinogram
   * is given.
   *****/
  if (emission && (atten != NULL) && (*backmlem == NULL))
    {
    if (verbose) printf("Backprojection of att (for ML-EM step)...\n");
    *backmlem = (float *) calloc(nrreconpixels, sizeof(float));
    
    p_backmlem = *backmlem;
    for (i = 0; i < nrreconpixels; i++)
      *p_backmlem++ = 0;

    for (i = 0; i < (prcf->nrproj); i++)
      NCproj_back(*backmlem, atten, i, prcf);

    p_backmlem = *backmlem;
    for (i = 0; i < nrreconpixels; i++)
      {
      if (*p_backmlem > 1e-6)
        *p_backmlem /= (prcf->nrproj);
      else
        *p_backmlem = 1.;
      p_backmlem++;
      }
    if (verbose) printf("done.\n");
    }

  /* If attenuation is available, precalculate the backprojection of the
   * attenuation coefficients over all projections.
   **********/
  if ((! prcf->backatten_ok))
    {
    if (verbose) printf("Backprojection of att/var...\n");

    p_worksino = prcf->worksino;
    p_atten    = atten;
    p_varsino  = variance;
    if (atten != NULL)
      {
      for (i = 0; i < nrsinopixels; i++)
        {
        *p_worksino++ = *p_atten * *p_atten / *p_varsino++;
        p_atten++;
        }
      }
    else
      {
      for (i = 0; i < nrsinopixels; i++)
        *p_worksino++ = 1.0 / *p_varsino++;
      }

    p_backatten = prcf->backatten;
    for (i = 0; i < nrreconpixels; i++)
      {
      *p_backatten++ = 0;
      }
    for (i = 0; i < prcf->nrproj; i++)
      {
      NCproj_back(prcf->backatten, prcf->worksino, i, prcf);
      }

    p_backatten = prcf->backatten;
    for (i = 0; i < nrreconpixels; i++)
      {
      if (*p_backatten > 1e-6)
        *p_backatten++ /= prcf->nrproj;
      else
        *p_backatten++ = 1.;
      }

    prcf->backatten_ok = 1;
    if (verbose) printf("done.\n");
    }

  /* Clear calcsino
  ****************/
  p_calcsino = prcf->calcsino;
  for (i = 0; i < nrsinopixels; i++)
    *p_calcsino++ = 0.0;

  /* Loop over iterations and subsets.
   **********/
  for (iter = 0; iter < nriter; iter++)
    {
    subsetnum = 0;
    for (subiter = 0; subiter < nrsubsets; subiter++)
      {
      subset = NCsino_subset(nrsubsets, prcf->nrproj, &subsetnum, &subsetmask,
                             &previous);
      NClsrecon(recon, sino, variance, atten, subset, relaxation, p_lik, 
                emission, backmlem, prcf);
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
   
  for (i = 0; i < prcf->nrproj; i++)
    NCproj_clearedge(recon, i, prcf);
  }
