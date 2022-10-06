/* module NCml */

/*
 general information: 
 This module applies a single MLEM (sub)iteration.
 */

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : mar 1996
  Modifications :
* endtopic History

******************************************************************************/
#define __NCml_module 1
#include "stdio.h"  /*NOINC*/
#include "NCproj_bproj.h"  /*NOINC*/
#include "math.h"  /*NOINC*/

/*enddeclars*/

/*-------------------------------------------------------------------------*/
/*global*/
void NCml(
  float       *recon,      /* reconstruction image */
  float       *sino,       /* sinogram */
  float       *atten,      /* attenuation sinogram or NULL */
  int         *subset,     /* subset or NULL (= all sinogram rows) */
  float       *likelihood, /* receives likelihood. Set to NULL if you don't
                              want to know. */
  p_ProjCoeff  prcf)       /* precalc. coefficients: see NCprojcoeff */


  {
  int     projnum;
  float   projinsubset;
  int    *isubset;
  int    *p_isubset;
  int     i;
  int     nrreconpixels;
  int     nrsinopixels;
  float  *p_recon;
  float  *p_calcsino;
  float  *p_atten;
  float  *p_worksino;
  float  *p_sino;
  float  *p_workrecon;
  float  *p_backatten;

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

  /* Project the current reconstruction, restricting the projection to the
   * subset. recon ==> calcsino.
   **********/
  p_isubset = isubset;
  while (*p_isubset > -1)
    {
    NCproj_forward(recon, prcf->calcsino, *p_isubset++, prcf);
    }

  /* Attenuate the projection if attenuation is provided.
   * (calcsino, atten) ==> calcsino
   **********/
  if (atten != NULL)
    {
    p_isubset = isubset;
    while (*p_isubset > -1)
      {
      p_calcsino = prcf->calcsino + *p_isubset * prcf->N;
      p_atten    = atten          + *p_isubset * prcf->N;
      for (i = 0; i < prcf->N; i++)
        {
        *p_calcsino++ *= *p_atten++;
        }
      p_isubset++;
      }
    }
 
  /* Divide measurement by computed projection.
   * (calcsino, sino) ==> worksino.
   **********/
  p_isubset = isubset;
  while (*p_isubset > -1)
    {
    p_calcsino = prcf->calcsino + *p_isubset * prcf->N;
    p_worksino = prcf->worksino + *p_isubset * prcf->N;
    p_sino     = sino           + *p_isubset * prcf->N;
    for (i = 0; i < prcf->N; i++)
      {
      if (*p_calcsino < SMALL)
        {
        *p_worksino++ = *p_sino++ / SMALL;
        p_calcsino++;
        }
      else
        {
        *p_worksino++ = *p_sino++ / *p_calcsino++;
        }
      }
    p_isubset++;
    }

  /* Backproject the resulting sinogram. If attenuation is provided,
   * the required operation is attenuated backprojection. This is
   * equivalent to first applying the attenuation and then apply the
   * backprojection.  Since the backprojector adds to the image values
   * instead of replacing them, the buffer must be set to zero first.
   * If attenuation is specified, every pixel must also be divided by
   * the backprojection of the attenuation coefficient.
   * (worksino, atten) ==> worksino   (attenuate)
   *   workrecon       ==> workrecon  (clear)
   *   worksino        ==> workrecon  (backprojection).
   *   atten           ==> backatten  (normalization for attenuation coeff.)
   **********/
  if (atten != NULL)
    {
    p_isubset = isubset;
    while (*p_isubset > -1)
      {
      p_worksino = prcf->worksino + *p_isubset * prcf->N;
      p_atten    = atten          + *p_isubset * prcf->N;
      for (i = 0; i < prcf->N; i++)
        {
        *p_worksino++ *= *p_atten++;
        }
      p_isubset++;
      }
    }
  
  nrreconpixels = prcf->N * prcf->N;
  p_workrecon = prcf->workrecon;
  for (i = 0; i < nrreconpixels; i++)
    {
    *p_workrecon++ = 0;
    }

  projinsubset = 0.0;
  p_isubset = isubset;
  while (*p_isubset > -1)
    {
    NCproj_back(prcf->workrecon, prcf->worksino, *p_isubset++, prcf);
    projinsubset++;
    }

 

  /* Backprojection of the attenuation only required if it has not been
   * done before.
   **********/
  if ((atten != NULL) && (! prcf->backatten_ok))
    {
    p_backatten = prcf->backatten;
    for (i = 0; i < nrreconpixels; i++)
      {
      *p_backatten++ = 0;
      }
    p_isubset = isubset;
    while (*p_isubset > -1)
      {
      NCproj_back(prcf->backatten, atten, *p_isubset++, prcf);
      }

    p_backatten = prcf->backatten;
    for (i = 0; i < nrreconpixels; i++)
      {
      if (*p_backatten > 1e-6)
        *p_backatten /= projinsubset;
      else
        *p_backatten = 1.;
      p_backatten++;
      }
    }

  /* Update the current reconstruction.
   * (recon, workrecon, backatten) ==> recon
   * If no attenuation is applied, the normalization reduces to division
   * by the number of projections.
   **********/
  p_recon     = recon;
  p_workrecon = prcf->workrecon;
  if (atten != NULL)
    {
    p_backatten = prcf->backatten;
    for (i = 0; i < nrreconpixels; i++)
      {
      *p_recon++ *= *p_workrecon++ / (projinsubset * *p_backatten++);
      }
    }
  else
    {
    for (i = 0; i < nrreconpixels; i++)
      {
      *p_recon++ *= *p_workrecon++ / projinsubset;
      }
    }

  /* Compute the likelihood if requested.
   **********/
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



