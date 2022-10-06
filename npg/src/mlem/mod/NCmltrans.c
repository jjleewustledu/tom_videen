/* module NCmltrans */

/*
 general information: 
 This module applies a single ml-gradient ascent (sub)iteration.
 Different from the MLEM method, we do not precompute the backprojection
 of the measurement once and for ever. The reason is that MLTR seems to
 be more sensitive to this computation: if subsets are applied, the 
 measurement must only be backprojected for the subsets in use.
 As a result, we have one projection and two backprojections in every
 iteration, so it is 1.5 times slower than MLEM.
 */

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : feb 1999.
  Modifications :
* endtopic History

******************************************************************************/
#define __NCmltrans_module 1
#include "stdio.h"  /*NOINC*/
#include "NCproj_bproj.h"  /*NOINC*/
#include "math.h"  /*NOINC*/

#define SMALLCOUNT 1e-3
#define SMALLSINO  1

/*enddeclars*/

/*-------------------------------------------------------------------------*/
/*global*/
void NCmltrans(
  float         *recon,       /* reconstruction image */
  float         *sino,        /* sinogram */
  float         *blank,       /* blank sinogram */
  float         *contam,      /* contamination sinogram or NULL */
  int           *subset,      /* subset or NULL (= all sinogram rows) */
  float          relaxation,  /* relaxation coeff. Convergence certain if 1. */
  float         *likelihood,  /* receives likelihood. Set to NULL if you don't
                               want to know. */
  p_ProjCoeff    prcf,        /* precalc. coefficients: see NCprojcoeff */
  p_AbsPrior     absprior,    /* Absolute intensity prior or NULL */
  p_MarkovPrior  markovprior, /* Markov prior or NULL */
  int            verbose) 

  {
  int     projnum;
  float   projinsubset;
  int    *isubset;
  int    *p_isubset;
  int     i;
  int     nrreconpixels;
  int     nrsinopixels;
  float   factor;
  float  *p_recon;
  float  *p_calcsino;
  float  *p_blank;
  float  *p_contam;
  float  *p_worksino;
  float  *p_sino;
  float  *p_workrecon;
  float  *p_backatten;
  float  *p_gprior;
  float  *p_g2prior;
  float   value;
  float   abspriorscale;
  float   markovscale;
  float   numerator;


  /* Initialization
  ***********/
  nrreconpixels = prcf->N * prcf->N;

  /* Generate default subset if no subset is provided.
   **********/
  if (subset == NULL)
    {
    isubset   = (int *) calloc(prcf->nrproj + 1, sizeof(int));
    p_isubset = isubset;
    for (projnum = 0; projnum < prcf->nrproj; projnum++)
      {
      *p_isubset++ = projnum;
      }
    *p_isubset = -1;
    }
  else
    {
    isubset = subset;
    }

  /* Project the current reconstruction, restricting to subset.
  **************************/
  if (verbose) printf("## projecting reconstruction\n");
  p_isubset = isubset;
  while (*p_isubset > -1)
    {
    NCproj_forward(recon, prcf->calcsino, *p_isubset++, prcf);
    }

  /* Attenuate the blank to predict the measurement, restricting
   * to the subset.
  **************************/
  projinsubset = 0.0;
  if (verbose) printf("## attenuate blank\n");
  p_isubset = isubset;
  while (*p_isubset > -1)
    {
    p_calcsino = prcf->calcsino + *p_isubset * prcf->N;
    p_blank    = blank          + *p_isubset++ * prcf->N;
    for (i = 0; i < prcf->N; i++)
      {
      *p_calcsino = *p_blank++ * exp(- *p_calcsino);
      p_calcsino++;
      }
    projinsubset++;
    }

  /* Update expression
  ********************
   * No contamination:
   *==================
   * rel    backp{calcproj  -  sino}
   * --- *  ------------------------
   *  N          backproj{calcproj}
   *
   * Contamination:
   *===============
   * rel   backp{calcproj - sino * calcproj / (calcproj + contam)}
   * --- * -------------------------------------------------------
   *  N                backp{ (sino - contam)^2 / sino}
   *
   * The numerator will be computed into WORKRECON
   * The denominator will be computed into BACKATTEN.
   * Intermediate sinograms are computed into WORKSINO.
   * So the update formula will be:
   * 
   * rel    WORKRECON
   * --- * -----------
   *  N     BACKATTEN
   *
   * If the priors are non-zero then we have to add the prior-gradient:
   *
   * rel     WORKRECON   +   (projinsub/nrangles) * priorweight * gprior
   * --- * -------------------------------------------------------------------
   *  N    BACKATTEN + 1/N * (projinsub/nrangles) * rel * priorweight * g2prior
   *
   * Note that when not all projections are used simultaneously, the
   * prior must be scaled down (or the likelihood scaled up), such that
   * the relative strength is the same as when all projections are used.
  ***************************/

  /* Numerator: Compute the sinogram to be backprojected
  **************/
  if (verbose) printf("## computing gradient of likelhood\n");
  if (contam == NULL)
    {
    p_isubset = isubset;
    while (*p_isubset > -1)
      {
      p_worksino = prcf->worksino + *p_isubset * prcf->N;
      p_calcsino = prcf->calcsino + *p_isubset * prcf->N;
      p_sino     = sino           + *p_isubset * prcf->N;
      for (i = 0; i < prcf->N; i++)
        *p_worksino++ = *p_calcsino++ - *p_sino++;
      p_isubset++;
      }
    }
  else
    {
    p_isubset = isubset;
    while (*p_isubset > -1)
      {
      p_worksino = prcf->worksino + *p_isubset * prcf->N;
      p_calcsino = prcf->calcsino + *p_isubset * prcf->N;
      p_contam   = contam         + *p_isubset * prcf->N;
      p_sino     = sino           + *p_isubset * prcf->N;
      for (i = 0; i < prcf->N; i++)
        {
        if (*p_calcsino > 0)  /* contam should be nonnegative */
          {
          numerator = *p_calcsino + *p_contam++;
          if (numerator < SMALLSINO) numerator = SMALLSINO;
          *p_worksino++ = *p_calcsino - *p_sino++ * *p_calcsino / 
                          numerator;
          }
        else
          {
          *p_worksino++ = 0;
          p_sino++;
          p_contam++;
          }
        p_calcsino++;
        }
      p_isubset++;
      }
   }

  /* Numerator: Backproject the sinogram into WORKRECON
  **************/
  p_workrecon = prcf->workrecon;
  for (i = 0; i < nrreconpixels; i++)
    *p_workrecon++ = 0;
  p_isubset = isubset;
  while (*p_isubset > -1)
    {
    NCproj_back(prcf->workrecon, prcf->worksino, *p_isubset++, prcf);
    }

  /* Denominator: Compute the sinogram to be backprojected.
   * If no contamination present, this is simply calcsino.
  **************/
  if (verbose) printf("## computing second derivative of likelhood\n");
  if (contam != NULL)
    {
    p_isubset = isubset;
    while (*p_isubset > -1)
      {
      p_worksino = prcf->worksino + *p_isubset * prcf->N;
      p_contam   = contam         + *p_isubset * prcf->N;
      p_sino     = sino           + *p_isubset * prcf->N;
      for (i = 0; i < prcf->N; i++)
        {
        if (*p_sino > SMALLSINO)
          {
          value = *p_sino - *p_contam++;
          *p_worksino++ = value * value / *p_sino++;
          }
        else
          {
          value = *p_sino++ - *p_contam++;
          *p_worksino++ = value * value / SMALLSINO;
          }
        }
      p_isubset++;
      }
    }
  /* Numerator: Backproject the sinogram into BACKATTEN
  **************/
  p_backatten = prcf->backatten;
  for (i = 0; i < nrreconpixels; i++)
    *p_backatten++ = 0;

  p_isubset = isubset;
  while (*p_isubset > -1)
    {
    if (contam == NULL)
      NCproj_back(prcf->backatten, prcf->calcsino, *p_isubset++, prcf);
    else
      NCproj_back(prcf->backatten, prcf->worksino, *p_isubset++, prcf);
    }

  if (absprior != NULL)
    {
    /* Prior: Check if gprior and g2prior have been allocated and
     *     allocate if necessary.
     **********/
    if (! absprior->allocated)
      {
      if (verbose)
        printf("## Allocating memory for absolute intensity prior\n");
      absprior->gprior  = (float *) calloc(nrreconpixels, sizeof(float));
      absprior->g2prior = (float *) calloc(nrreconpixels, sizeof(float));
      absprior->allocated = 1;
      }

    /* Call NCgrad_prior to compute gprior and g2prior.   
    ********/
    if (verbose) printf("## Computing absolute intensity prior\n");
    NCgradprior(absprior->gprior, absprior->g2prior, recon, nrreconpixels,
                absprior->number, absprior->mean, absprior->std,
                absprior->width);

    /* Update WORKRECON and BACKATTEN
    *********/
    abspriorscale = absprior->weight * projinsubset / ((float) prcf->nrproj);
    p_workrecon = prcf->workrecon;
    p_gprior    = absprior->gprior;
    for (i = 0; i < nrreconpixels; i++)
      *p_workrecon++ += abspriorscale * *p_gprior++;

    abspriorscale *= relaxation / prcf->N;
    p_backatten = prcf->backatten;
    p_g2prior   = absprior->g2prior;
    for (i = 0; i < nrreconpixels; i++)
      *p_backatten++ += abspriorscale * *p_g2prior++;
    }

  if (markovprior != NULL)
    {
    /* Prior: Check if gprior has been allocated, and allocate if not.
     **********/
    if (! markovprior->allocated)
      {
      if (verbose)
        printf("## Allocating memory for Markov prior\n");
      markovprior->gprior = (float *) calloc(nrreconpixels, sizeof(float));
      markovprior->allocated = 1;
      }

     /* Call NCmarkov to compute gprior and g2prior.   
    ********/
    if (verbose) printf("## Computing Markov prior\n");
    NCmarkov(markovprior->gprior, &(markovprior->g2prior),
             recon, prcf->N, prcf->N, markovprior->delta,
             markovprior->neighborhood, markovprior->functiontype);

    /* Update WORKRECON and BACKATTEN
    *********/
    markovscale = markovprior->weight * projinsubset / ((float) prcf->nrproj);
    p_workrecon = prcf->workrecon;
    p_gprior    = markovprior->gprior;
    for (i = 0; i < nrreconpixels; i++)
      *p_workrecon++ += markovscale * *p_gprior++;
   
    markovscale *= relaxation * markovprior->g2prior / prcf->N;
    p_backatten = prcf->backatten;
    for (i = 0; i < nrreconpixels; i++)
      *p_backatten++ += markovscale;
    }

  /* Compute the update expression in the WORKRECON buffer.
   * Update expression is:
   ********/
  p_workrecon = prcf->workrecon;
  p_backatten = prcf->backatten;
  for (i = 0; i < nrreconpixels; i++)
    {
    if (*p_backatten > SMALLCOUNT)
      *p_workrecon++ /= *p_backatten++;
    else
      {
      *p_workrecon++ /= SMALLCOUNT;
      p_backatten++;
      }
    }


  /* Add the update to the reconstruction
  *********/
  factor      = relaxation / prcf->N;
  p_recon     = recon;
  p_workrecon = prcf->workrecon;
  for (i = 0; i < nrreconpixels; i++)
    {
    *p_recon += factor * *p_workrecon++;
    if (*p_recon < 0) *p_recon = 0;
    p_recon++;
    }

  for (i = 0; i < prcf->nrproj; i++)
    {
    NCproj_clearedge(recon, i, prcf);
    }

  /* Compute the likelihood if requested
  *********************/
  if (likelihood != NULL)
    {
    *likelihood = 0.0;
    p_calcsino = prcf->calcsino;
    p_sino     = sino;
    nrsinopixels = prcf->nrproj * prcf->N; 
    for (i = 0; i < nrsinopixels; i++)
      {
      if (*p_calcsino > VERYSMALL)
        {
        *likelihood += - *p_calcsino + *p_sino++ * log(*p_calcsino);
        p_calcsino++;
        }
      else
        {
        *likelihood += - VERYSMALL + *p_sino++ * log(VERYSMALL);
        p_calcsino++;
        }
      }
    }
  
  /* Clean up
   **********/
  if (subset == NULL)
    {
    free(isubset);
    }
  }


