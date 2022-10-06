/* module NCsino_subset */

/*
 general information: 
 This module computes the sinograms row belonging to the next subset.
 */

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : feb 1996
  Modifications :
* endtopic History

******************************************************************************/
#define __NCsino_subset_module 1
#include "stdio.h"  /*NOINC*/
#include "NCproj_bproj.h"  /*NOINC*/
#include "math.h"  /*NOINC*/

/*enddeclars*/

/*-------------------------------------------------------------------------*/
/*global*/
int* NCsino_subset(  /* Returns an newly allocated array of indices. 
                        The value following the last subsets is -1. */
  int    nrsubsets,  /* the number of subsets */
  int    nrangles,   /* the number of projection angles == nr sinogram rows */
  int   *subsetnum,  /* The current subset number (incremented afterwards).
                        Should be set to zero for first call */
  int  **subsetmask, /* Internal variable, allocated and freed internally */
  int   *previous    /* Internal variable, set by NCsino_subset. */
  )

  {
  int   *subset;
  int    subsetlength;
  int    i, keer;
  int   *p_subset;
  int   *p_subsetmask;
  int   *distarray;
  int    distance;
  int    disttoprevious;
  int    current;
  int    curdist;
  int    curdisttoprev;

  if (*subsetnum == 0)
    {
    subsetlength = (int)  ceil(((float) nrangles) / nrsubsets);
    *subsetmask  = (int*) calloc(nrsubsets, sizeof(int));
    subset       = (int*) calloc(subsetlength+1, sizeof(int));

    p_subset     = subset;
    p_subsetmask = *subsetmask;
    for (i = 0; i < subsetlength; i++)
      {
      *p_subset++     = i * nrsubsets;
      }
    *p_subset = -1;
    for (i = 0; i < nrsubsets; i++)
      {
      *p_subsetmask++ = 0;
      }
    **subsetmask = 1;
    *previous    = 0;
    }

  else
    {
    p_subsetmask = *subsetmask;
    distarray = (int*) calloc(nrsubsets, sizeof(int));

    /* distarray will store the distance of each point to the closest left
     * neighbor. Because the array should be regarded as cyclic, the procedure
     * is carried out twice.
     ********/
    distance = 3000;
    for (keer = 0; keer < 2; keer++)
      {
      for (i = 0; i < nrsubsets; i++)
        {
        if (p_subsetmask[i] == 1)
          distance = 0;
        else
          distance += 1;
        distarray[i] = distance;
        }
      }

    /* Repeat the procedure now going from right to left. Distance is only
     * updated if the distance to the right neighbor is smaller than the
     * the distance we found previously for the left neighbor.
     ********/
    distance = 3000;
    for (keer = 0; keer < 2; keer++)
      {
      for (i = nrsubsets-1; i >= 0; i--)
        {
        if (p_subsetmask[i] == 1)
          distance = 0;
        else
          distance += 1;
        if (distarray[i] > distance)
          distarray[i] = distance;
        }
      }

    /* Distarray now contains the distance to the closest marked subset.
     * We now want the subset with the maximum distance. If there are many
     * of these, we want the one most different from the previous subset.
     ********/
    distance = -1;
    for (i = 0; i < nrsubsets; i++)
      {
      if (distarray[i] > distance)
        {
        current        = i;
        distance       = distarray[i];
        curdisttoprev  = nrsubsets - abs(current - *previous);
        if (abs(current - *previous) < curdisttoprev)
          curdisttoprev = abs(current - *previous);
        }
      else if (distarray[i] == distance)
        {
        disttoprevious = nrsubsets - abs(i - *previous);
        if (abs(i - *previous) < disttoprevious)
          disttoprevious = abs(i - *previous);
        
        if (disttoprevious > curdisttoprev)
          {
          current = i;
          curdisttoprev = disttoprevious;
          }
        }
      }

    *previous    = current;   
    subsetlength = (int)  ceil(((float) nrangles) / nrsubsets);
    subset       = (int*) calloc(subsetlength+1, sizeof(int));
    p_subset     = subset;
    *(*subsetmask + current) = 1;
    for (i = current; i < nrangles; i += nrsubsets)
      {
      *p_subset++     = i;
      }
    *p_subset = -1;
    }

  *subsetnum += 1;
  if (*subsetnum >= nrsubsets) *subsetnum = 0;
  if (*subsetnum == 0)
    {
    free(*subsetmask);
    *subsetmask = NULL;
    }
  return(subset);
  }



