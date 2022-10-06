/* module NCml_spect */

/*
 general information: 
 This module computes the sinograms row belonging to the next subset.
 */

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : mar 1996
  Modifications :
* endtopic History

******************************************************************************/
#define __NCml_spect_module 1
#include "stdio.h"  /*NOINC*/
#include "NCproj_bproj.h"  /*NOINC*/
#include "math.h"  /*NOINC*/

/*enddeclars*/

/*-------------------------------------------------------------------------*/
/*global*/
void NCml_spect(
  float       *recon,      /* reconstruction image */
  float       *sino,       /* sinogram */
  float       *sensit,     /* sensitivity sinogram or NULL. If set, should
                              contain values <= 1. */
  float       *attenimg,   /* attenuation map image (att.coeff per pixelsize),
                              or NULL (= no attenuation) */
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
  float  *all_one;
  float  *p_recon;
  float  *p_calcsino;
  float  *p_sensit;
  float  *p_worksino;
  float  *p_sino;
  float  *p_workrecon;
  float  *p_backsensit;

  isubset = NULL;
  all_one = NULL;

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
  if (attenimg == NULL)
    {
    while (*p_isubset > -1)
      NCproj_forward(recon, prcf->calcsino, *p_isubset++, prcf);
    }
  else
    {
    while (*p_isubset > -1)
      NCproj_attforward(recon, prcf->calcsino, attenimg, *p_isubset++, prcf);
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

  /* Backproject the resulting sinogram. 
   * Since the backprojector adds to the image values
   * instead of replacing them, the buffer must be set to zero first.
   * If attenuation is specified, every pixel must also be divided by
   * the backprojection of the attenuation coefficient. Idem if sensitivity
   * is provided.
   *   workrecon ==> workrecon  (clear)
   *   worksino  ==> workrecon  (backprojection).
   *   sensit    ==> backsensit  (normalization for attenuation and
   *                              sensitivity coeff.)
   **********/
  nrreconpixels = prcf->N * prcf->N;
  p_workrecon = prcf->workrecon;
  for (i = 0; i < nrreconpixels; i++)
    {
    *p_workrecon++ = 0;
    }

  projinsubset = 0.0;
  p_isubset = isubset;
  if (attenimg == NULL)
    {
    while (*p_isubset > -1)
      {
      NCproj_back(prcf->workrecon, prcf->worksino, *p_isubset++, prcf);
      projinsubset++;
      }
    }
  else
    {
    while (*p_isubset > -1)
      {
      NCproj_attback(prcf->workrecon, prcf->worksino, attenimg, 
                     *p_isubset++, prcf);
      projinsubset++;
      }
    }

  /* Compute the sum of the weights in every image pixel. This is done
   * by attenuated backprojection of the sensitivity matrix. If no
   * attenuation map is given, unattenuated backprojection is used. If
   * no sensitivity is available, an image of all ones is used. If
   * both are not given, the sum of weights equals the number of
   * subsets and the whole operation can be skipped.
   * If prcf->backatten_ok is set, this means that some other routine filled
   * the matrix with appropriate values. In that case, computation of
   * backatten can be skipped.
   * Note that backatten_ok is not set in this routine, because backatten
   * is probably not valid in the next call, since the number of subsets
   * is likely to be different.
   **********/
  if (((sensit != NULL) || (attenimg != NULL)) && (! prcf->backatten_ok))
    {
    /* Generate a sensitivity sinogram containing only ones, if nothing else
     * is available. The pointer p_sensit points to what will be used.
     **************/
    if (sensit == NULL)
      {
      all_one = (float *) calloc(nrreconpixels, sizeof(float));
      p_sensit = all_one;
      for (i = 0; i < nrreconpixels; i++)
        {
        *p_sensit++ = 1.0;
        }
      p_sensit = all_one;
      }
    else
      {
      p_sensit = sensit;
      }
    
    /* Set backprojection image to zero 
     ***************/
    p_backsensit = prcf->backatten;
    for (i = 0; i < nrreconpixels; i++)
      {
      *p_backsensit++ = 0;
      }
    
    /* Attenuated or unattenuated backprojection of the sensitivity     
    ***************/
    p_isubset = isubset;
    if (attenimg == NULL)
      {
      while (*p_isubset > -1)
        {
        NCproj_back(prcf->backatten, p_sensit, *p_isubset++, prcf);
        }
      }
    else
      {
      while (*p_isubset > -1)
        {
        NCproj_attback(prcf->backatten, p_sensit, attenimg, 
                       *p_isubset++, prcf);
        }
      }
    
    /* Divide each value by the number of projections in the current    
     * subset.  
     ***********/
    p_backsensit = prcf->backatten;
    for (i = 0; i < nrreconpixels; i++)
      {
      if (*p_backsensit > 1e-6)
        *p_backsensit /= projinsubset;
      else
        *p_backsensit = 1.;
      p_backsensit++;
      }
    }
  /* Update the current reconstruction.
   * (recon, workrecon, backsensit) ==> recon
   * If no attenuation is applied, the normalization reduces to division
   * by the number of projections.
   **********/
  p_recon     = recon;
  p_workrecon = prcf->workrecon;
  if ((sensit != NULL) || (attenimg != NULL))
    {
    p_backsensit = prcf->backatten;
    for (i = 0; i < nrreconpixels; i++)
      {
      *p_recon++ *= *p_workrecon++ / (projinsubset * *p_backsensit++);
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
  if (all_one != NULL)
    {
    free(all_one);
    }
  }



