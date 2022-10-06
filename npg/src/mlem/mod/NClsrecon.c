/* module NClsrecon */

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
#define __NClsrecon_module 1
#include "stdio.h"  /*NOINC*/
#include "NCproj_bproj.h"  /*NOINC*/
#include "math.h"  /*NOINC*/

/*enddeclars*/
#define SMALLVALUE 1.0
/*-------------------------------------------------------------------------*/
/*global*/
  /* Implements: 
   *
   *                         relaxation    backproj((y_i - r_i) / var_i)
   *  lambda_j = lambda_j + ------------  ----------------------------
   *                         nrcols        backproj(sensit_i / var_i)
   *
   * lambda_j: reconstruction value
   * y_i:      measured count
   * var_i:    variance (for emission = count)
   * sensit_i  sensitivity: for PET: 1 / (atten.corr x normalization)
   *
   * if emission is set, the module switches to 
   *
   *                                       backproj((y_i - r_i) / var_i)
   *  lambda_j = lambda_j +  lambda_j  *   ----------------------------
   *                                           backproj(sensit_i)
   *
   * whenever the update of the second expression is larger than that of
   * the first.
   * This expression is identical to ML-EM, if var_i equals the
   * computed sinogram. If emission is set, var_i is replaced with r_i.
   *
   ********/
void NClsrecon(
  float       *recon,      /* reconstruction image */
  float       *sino,       /* sinogram with measured counts*/
  float       *variance,   /* sinogram with variance values. Should be
                              strictly positive, because in denominator */
  float       *atten,      /* attenuation sinogram or NULL */
  int         *subset,     /* subset or NULL (= all sinogram rows) */
  float       relaxation,  /* relaxation value. In the order of 1..2 */
  float       *likelihood, /* receives likelihood. Set to NULL if you don't
                              want to know. */
  int         emission,    /* When set, mimics MLEM for large pixels */
  float     **backmlem,    /* Only used when emission is set. Allocated
                              internally if necessary. */
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
  float  *p_varsino;
  float  *p_atten;
  float  *p_worksino;
  float  *p_sino;
  float  *p_workrecon;
  float  *p_backatten;
  float  *p_backmlem;
  float  schaal;
  float  weight1, weight2;

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
 
  /* Compute weighted difference.
   * (calcsino, sino) ==> worksino.
   * Attenuate if attenuation is provided.
   **********/
  p_isubset = isubset;
  while (*p_isubset > -1)
    {
    p_calcsino = prcf->calcsino + *p_isubset * prcf->N;
    p_worksino = prcf->worksino + *p_isubset * prcf->N;
    p_sino     = sino           + *p_isubset * prcf->N;
    p_atten    = atten          + *p_isubset * prcf->N;

    if (emission)
      p_varsino  = prcf->calcsino + *p_isubset * prcf->N;
    else
      p_varsino  = variance       + *p_isubset * prcf->N;

    if (atten != NULL)
      {
      for (i = 0; i < prcf->N; i++)
        {
        if (*p_varsino < SMALLVALUE)
          {
          *p_worksino++ 
            = ((*p_sino++ - *p_calcsino++) / SMALLVALUE) * *p_atten++ ;
          p_varsino++;
          }
        else
          {
          *p_worksino++ 
            = ((*p_sino++ - *p_calcsino++) / *p_varsino++) * *p_atten++;
          }
        }
      }
    else
      {
      for (i = 0; i < prcf->N; i++)
        {
        if (*p_varsino < SMALLVALUE)
          {
          *p_worksino++ 
            = (*p_sino++ - *p_calcsino++) / SMALLVALUE;
          p_varsino++;
          }
        else
          {
          *p_worksino++ 
            = (*p_sino++ - *p_calcsino++) / *p_varsino++;
          }
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
  nrreconpixels = prcf->N * prcf->N;
  nrsinopixels = prcf->nrproj * prcf->N; 
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
  p_workrecon = prcf->workrecon;
  for (i = 0; i < nrreconpixels; i++)
    {
    *p_workrecon++ /=projinsubset;
    }

  /* ML-EM normalization if emission is set, and if an attenuation sinogram
   * is given.
   *****/
  if (emission && (atten != NULL) && (*backmlem == NULL))
    {
    printf(",\n");
    *backmlem = (float *) calloc(nrreconpixels, sizeof(float));
    
    p_backmlem = *backmlem;
    for (i = 0; i < nrreconpixels; i++)
      *p_backmlem++ = 0;

    p_isubset = isubset;
    while (*p_isubset > -1)
      {
      NCproj_back(*backmlem, atten, *p_isubset++, prcf);
      }
    
    p_backmlem = *backmlem;
    for (i = 0; i < nrreconpixels; i++)
      {
      if (*p_backmlem > 1e-6)
        *p_backmlem /= projinsubset;
      else
        *p_backmlem = 1.;
      p_backmlem++;
      }
    }

  /* Backprojection of the attenuation only required if it has not been
   * done before.
   **********/
  if (! prcf->backatten_ok)
    {
    printf(".\n");

    /* Compute the sinogram: attenuation / variance.
    ******************/
    p_isubset = isubset;
    while (*p_isubset > -1)
      {
      p_worksino = prcf->worksino + *p_isubset * prcf->N;
      p_atten    = atten          + *p_isubset * prcf->N;
      p_varsino  = variance       + *p_isubset * prcf->N;
      if (atten != NULL)
        {
        for (i = 0; i < prcf->N; i++)
          *p_worksino++ = *p_atten * *p_atten++ / *p_varsino++;
        }
      else
        {
        for (i = 0; i < prcf->N; i++)
          *p_worksino++ = 1.0 / *p_varsino++;
        }
      p_isubset++;
      }

    p_backatten = prcf->backatten;
    for (i = 0; i < nrreconpixels; i++)
      {
      *p_backatten++ = 0;
      }

    p_isubset = isubset;
    while (*p_isubset > -1)
      {
      NCproj_back(prcf->backatten, prcf->worksino, *p_isubset++, prcf);
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
  p_backatten = prcf->backatten;
  p_backmlem  = *backmlem;
  schaal = relaxation / prcf->N;
  if (emission)
    {
    for (i = 0; i < nrreconpixels; i++)
      {
      weight1 =  schaal  / *p_backatten++;  /* Least squares heuristic step */
      if (*backmlem == NULL)
        weight2 =  *p_recon;
      else
        weight2 =  *p_recon / *p_backmlem++;  /* ML-EM step */
      if (weight1 > weight2)
            /* use largest step, thus allowing negatives */
        {
        *p_recon++ += weight1 * *p_workrecon++;
        }
      else
        {
        *p_recon++ += weight2 * *p_workrecon++;
        }
      }
    }
  else
    {
    for (i = 0; i < nrreconpixels; i++)
      {
      *p_recon++ += schaal * *p_workrecon++ / *p_backatten++;
      }
    }

  /* Compute the likelihood if requested.
   **********/
  if (likelihood != NULL)
    {
    *likelihood = 0.0;
    p_calcsino = prcf->calcsino;
    p_sino     = sino;
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



