/* module NCproj_free */

/*
 general information: 
 This module deallocates the structure with arrays allocated by NCprojcoeff.
 */

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : feb 1996
  Modifications :
* endtopic History

******************************************************************************/
#define __NCproj_free_module 1
#include "stdio.h"  /*NOINC*/
#include "NCproj_bproj.h"  /*NOINC*/

/*enddeclars*/

/*-------------------------------------------------------------------------*/
/*global*/
void NCproj_free(p_ProjCoeff *projcoeff)

  {
  p_ProjCoeff  prcf;
  int          projnum, nrproj;


  prcf   = *projcoeff;
  nrproj = prcf->nrproj;

  for (projnum = 0; projnum < nrproj; projnum++)
    {
    prcf = *projcoeff + projnum;

    free(prcf->recindex1);
    free(prcf->recindex2);
    free(prcf->recweight1);
    free(prcf->recweight2);
    free(prcf->bigsino);
    free(prcf->linefirst1);
    free(prcf->linefirst2);
    free(prcf->linelast1);
    free(prcf->linelast2);
    free(prcf->lineoffset);
    free(prcf->bigindex1);
    free(prcf->bigindex2);
    free(prcf->bigweight1);
    free(prcf->bigweight2);
    free(prcf->smallindex1);
    free(prcf->smallindex2);
    free(prcf->smallweight1);
    free(prcf->smallweight2);
    }
  free(prcf->worksino);
  free(prcf->calcsino);
  free(prcf->workrecon);
  free(prcf->backatten);
  free(*projcoeff);
  *projcoeff = NULL;
  }


