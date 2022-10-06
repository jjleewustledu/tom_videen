/* module NCprojcoeff3 */

/*
 general information: 
 This module allocates a structure and fills it with precalculated
 interpolation coefficients for projection and backprojection.
 The module is identical to NCprojcoeff, except that it accepts an array
 with center of rotation offsets as last parameter. If this pointer is
 set to NULL, the module behaves exactly the same as NCprojcoeff. If the
 array not NULL, it is assumed point to an array of nrproj floating point
 elements, containing the center of rotation offset in pixels for that
 angle. Angle dependent offsets are required for camera's supporting
 bed movements to minimize patient to detector distance.
 */

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : jan 1997
  Modifications :
* endtopic History

******************************************************************************/
#define __NCprojcoeff3_module 1
#include "stdio.h"  /*NOINC*/
#include "NCproj_bproj.h"  /*NOINC*/
#include "math.h"  /*NOINC*/

/*enddeclars*/


#define NCPROJCOEFF_VALUES \
  curcol    = colarray[pix] + coloffset; \
  currow    = rowarray[pix] + rowoffset; \
  coldist   = curcol - center; \
  rowdist   = currow - center; \
  distance2 = coldist * coldist + rowdist * rowdist



/*-------------------------------------------------------------------------*/
/*global*/
p_ProjCoeff NCprojcoeff3(
  int   nrdet,      /* Horizontal sinogram dimension */
  int   nrproj,     /* Vertical sinogram dimension */
  float startangle, /* first angle in radians: zero is proj along y-axis */
  float deltaangle, /* last angle in radians. Positive is clockwise. */
  float radius,     /* radius of the FOV in pixels. Use -1 for the entire
                       image */
  float *coroffset, /* center of rotation offset in pixels. Ingored when
                       NULL pointer */
  float  arc_radius /* radius of the detector surface in pixels. This is
                       intended for arc-correction in PET. Ignored when <= 0,
                       so set it to zero for planar detectors. */
  )
  
  {
  p_ProjCoeff   ProjCoeffArray;
  p_ProjCoeff   prcf;
  int           projnum;
  int           i;
  int           index;
  int           startrow, stoprow, rowoffset;
  int           irow, row0, row1, icol, col0, col1;
  int           startcol, stopcol, coloffset;
  int           *rowarray1;
  int           *rowarray2;
  int           *colarray1;
  int           *colarray2;
  int           line;
  int           ibigcenter;
  int           pix;
  int           curcol1, curcol2, currow1, currow2;
  int           first, last;
  float         col, row;
  float         angle;
  float         scale;
  float         center;
  float         bigcenter, smallcenter;
  float         bigcoord,  smallcoord;
  float         slope;
  float        *ptrcor;
  float         offset;
  float         finalscale, arc;

  /* Allocate array: one structure per projection angle */
  ProjCoeffArray =  (p_ProjCoeff) calloc(nrproj, sizeof(s_ProjCoeff));

  /* fill in the structure for all projection angles. */
  
  offset = 0.0;
  ptrcor = coroffset;
  
  for (projnum = 0; projnum < nrproj; projnum++)
    {
    if (ptrcor != NULL)
   	  offset = *ptrcor++;

    prcf = ProjCoeffArray + projnum;
    prcf->nrproj = nrproj;
    
    /* angle, scale, cosscale
     *************************/
    angle =  startangle + projnum * deltaangle;
    scale =  fabs(sin(angle));
    if (scale < fabs(cos(angle)))
      {
      scale = fabs(cos(angle));
      prcf->cosscale = 1;
      }
    else
      {
      prcf->cosscale = 0;
      }
    prcf->angle = angle;
    prcf->scale = scale;

    /* N, bigN.
     * bigN is guaranteed to be odd.
     *********/
    prcf->N     = nrdet;
    prcf->bigN  = (int) (nrdet / (2 * scale) + 1) * 2 + 1;
    smallcenter = (prcf->N    - 1.0) / 2.0 + offset;
    bigcenter   = (prcf->bigN - 1.0) / 2.0;

    /* Allocate the structures for this angle
     *****/
    prcf->recindex1    = (int *) calloc(prcf->N,    sizeof(int));
    prcf->recindex2    = (int *) calloc(prcf->N,    sizeof(int));
    prcf->recweight1   = (float *) calloc(prcf->N,    sizeof(float));
    prcf->recweight2   = (float *) calloc(prcf->N,    sizeof(float));
    prcf->bigsino      = (float *) calloc(prcf->bigN, sizeof(float));
    prcf->linefirst1   = (int *) calloc(prcf->bigN, sizeof(int));
    prcf->linefirst2   = (int *) calloc(prcf->bigN, sizeof(int));
    prcf->linelast1    = (int *) calloc(prcf->bigN, sizeof(int));
    prcf->linelast2    = (int *) calloc(prcf->bigN, sizeof(int));
    prcf->lineoffset   = (int *) calloc(prcf->bigN, sizeof(int));
    prcf->bigindex1    = (int *) calloc(prcf->N,    sizeof(int));
    prcf->bigindex2    = (int *) calloc(prcf->N,    sizeof(int));
    prcf->bigweight1   = (float *) calloc(prcf->N,    sizeof(float));
    prcf->bigweight2   = (float *) calloc(prcf->N,    sizeof(float));
    prcf->smallindex1  = (int *) calloc(prcf->bigN, sizeof(int));
    prcf->smallindex2  = (int *) calloc(prcf->bigN, sizeof(int));
    prcf->smallweight1 = (float *) calloc(prcf->bigN, sizeof(float));
    prcf->smallweight2 = (float *) calloc(prcf->bigN, sizeof(float));

    /* Allocate local structures, needed to compute linefirst1, ...
     ******/
    colarray1 = (int *) calloc(prcf->N, sizeof(int));
    colarray2 = (int *) calloc(prcf->N, sizeof(int));
    rowarray1 = (int *) calloc(prcf->N, sizeof(int));
    rowarray2 = (int *) calloc(prcf->N, sizeof(int));
   
    /* Indices and weights required to compute a regular sinogram line
     * (N points) from an oversampled one (bigN points).
     * The weights must be divided by scale here: we want to keep the number
     * of counts in the sinogram line constant. Since the number of pixels
     * is decreased, the number of counts must be increased accordingly.
     *
     * If arc-correction is required, the following formula is applied:
     * small(x) = big(R sin(x/R) / scale) * cos(x/R) / scale,
     *
     * where R is the arc radius and x is (i - smallcenter)
     * The cos scales for increased total counts due to increasing the
     * number of pixels by the arc-correction, R sin(x/R) is the distance of
     * LOR x from the center of the FOV.
     *********/
    for (i = 0; i < prcf->N; i++)
      {
      if (arc_radius <= 0.0)
        {
        bigcoord   = (i - smallcenter) / scale + bigcenter;
        finalscale = 1./scale;
        }
      else
        {
        arc        = (i - smallcenter) / arc_radius;
        bigcoord   = arc_radius * sin(arc) / scale + bigcenter;
        if (fabs(arc) < M_PI_2)
          finalscale = cos(arc) / scale;
        else
          finalscale = 0.0;
        }
      prcf->bigindex1[i]  = (int) floor(bigcoord);
      prcf->bigindex2[i]  = prcf->bigindex1[i] + 1;
      prcf->bigweight1[i] = (prcf->bigindex2[i] - bigcoord) * finalscale;
      prcf->bigweight2[i] = (bigcoord - prcf->bigindex1[i]) * finalscale;

      /* check whether some indices go out of range. If so, give them
         zero weight: we assume that all pixels outside the image are zero */
      if ((prcf->bigindex1[i] < 0) ||
          (prcf->bigindex1[i] >= prcf->bigN))
        {
        prcf->bigindex1[i]  = 0;   /* certainly a legal index */
        prcf->bigweight1[i] = 0.0; /* zero weight: a zero pixel value */
        }
      if ((prcf->bigindex2[i] < 0) || 
          (prcf->bigindex2[i] >= prcf->bigN))
        {
        prcf->bigindex2[i]  = 0;   /* certainly a legal index */
        prcf->bigweight2[i] = 0.0; /* zero weight: a zero pixel value */
        }
      }

    /* Indices and weights required to compute an oversampled sinogram
     * line (bigN points) from a regular sinogram line (N points).
     * Here we must NOT multiply the weights with scale! The
     * oversampled pixelvalues are used as interpolation value for the
     * backprojection, so here we want to keep the pixel values
     * constant, not the total number of counts. Appropriate scaling
     * is in fact done automatically by the sampling scheme used in
     * the backprojector.
     *
     * If arc-correction is required, the following formula is applied:
     * big(x) = small(R asin(scale*x / R) / cos(asin(scale*x / R))
     *                           or
     * big(x) = small(R asin(scale*x / R) * sqrt(1 - (scale*x/R)^2)
     *
     * where R is the arc radius and x is (i - bigcenter).
     * The data should not be scaled up for the "un-arccorrection":
     * we implement here the transpose of the arccorrection, not the inverse.
     *********/
    for (i = 0; i < prcf->bigN; i++)
      {
      if (arc_radius <= 0)
        {
        smallcoord = (i - bigcenter) * scale + smallcenter;
        finalscale = 1;
        }
      else
        {
        arc        = scale * (i - bigcenter) / arc_radius;
        if (fabs(arc) <= 1.0)
          smallcoord = arc_radius * asin(arc) + smallcenter;
        else
          smallcoord =  arc_radius * asin(arc / fabs(arc)) + smallcenter;
        if ((arc*arc) > 0.99)
          finalscale = 0.0;
        else
          finalscale = 1.;
        }
      prcf->smallindex1[i]  = (int) floor(smallcoord);
      prcf->smallindex2[i]  = prcf->smallindex1[i] + 1;
      prcf->smallweight1[i] = (prcf->smallindex2[i] - smallcoord) * finalscale;
      prcf->smallweight2[i] = (smallcoord - prcf->smallindex1[i]) * finalscale;
      if ((prcf->smallindex1[i] < 0) || 
          (prcf->smallindex1[i] >= prcf->N))
        {
        prcf->smallindex1[i]  = 0;   /* certainly a legal index */
        prcf->smallweight1[i] = 0.0; /* zero weight: a zero pixel value */
        }
      if ((prcf->smallindex2[i] < 0) ||
          (prcf->smallindex2[i] >= prcf->N))
        {
        prcf->smallindex2[i]  = 0;   /* certainly a legal index */
        prcf->smallweight2[i] = 0.0; /* zero weight: a zero pixel value */
        }
      }

    /* Compute the indices and weights for interpolation along
     * projection/ backprojection lines. If cos(angle) > sin(angle)
     * then cosscale equals unity, and the projection line is "more
     * vertical than horizontal".  In this case, there is exactly one
     * (real) point per row, and interpolation is done between the two
     * nearest neighboring pixels on that row.
     *******/
    center = (prcf->N    - 1.0) / 2.0;
    if (prcf->cosscale)
      {
      /* Compute the first row, the rowoffset and the "stop row", which is 
       * last row + the rowoffset.
       */
      if (cos(angle) > 0)
        {
        startrow  = 0;
        stoprow   = prcf->N;
        rowoffset = 1;
        }
      else
        {
        startrow  = prcf->N - 1;
        stoprow   = -1;
        rowoffset = -1;
        }
      index = 0;
      slope = sin(angle) / cos(angle);
      for (irow = startrow; irow != stoprow; irow += rowoffset)
        {
        col  = center - slope * (irow - center);
        col0 = (int) floor(col);
        col1 = col0 + 1;
        prcf->recindex1[index]  = col0 + prcf->N * irow;
        prcf->recindex2[index]  = col1 + prcf->N * irow;
        prcf->recweight1[index] = col1 - col;
        prcf->recweight2[index] = col  - col0;

        rowarray1[index] = irow;
        rowarray2[index] = irow;
        colarray1[index] = col0;
        colarray2[index] = col1;
        index++;
        }
      }
    else
      {
      if (sin(angle) < 0)
        {
        startcol  = 0;
        stopcol   = prcf->N;
        coloffset = 1;
        }
      else
        {
        startcol  = prcf->N - 1;
        stopcol   = -1;
        coloffset = -1;
        }
      index = 0;
      slope = cos(angle) / sin(angle);
      for (icol = startcol; icol != stopcol; icol += coloffset)
        {
        row = center - slope * (icol -  center);
        row0 = (int) floor(row);
        row1 = row0 + 1;
        prcf->recindex1[index]  = icol + row0 * prcf->N;
        prcf->recindex2[index]  = icol + row1 * prcf->N;
        prcf->recweight1[index] = row1 - row;
        prcf->recweight2[index] = row  - row0;
        rowarray1[index] = row0;
        rowarray2[index] = row1;
        colarray1[index] = icol;
        colarray2[index] = icol;
        index++;
        }
      }

    /* Calculation of the offsets, used to shift the central projection line
     * to another parallel line. The weights stay the same, the addresses
     * change only by that offset. However, the intersection of this shifted
     * projection line with the image depends on the shift. The intersection
     * is maximum for the central line, so we only need to know which portion
     * of the weights and addresses must be used for the shifted line.
     * Since each point is computed as the interpolation of the two neighboring
     * pixels, there is an additional complication: it is possible that one
     * of these points is in the image, while the other is not. Consequently,
     * we need to start-indices and two stop-indices, one for the leftmost
     * (or upper) pixel, one for the rightmost (or lower) pixel.
     ********/
    ibigcenter = (int) bigcenter;  /* no problem since bigN is odd */
    for (line = 0; line < prcf->bigN; line++)
      {
      prcf->linefirst1[line] = -1;
      prcf->linefirst2[line] = -1;
      prcf->linelast1[line]  = -2;
      prcf->linelast2[line]  = -2;

      if (prcf->cosscale)
        {
        rowoffset = 0;
        if (cos(angle) > 0)  
          {coloffset = line - ibigcenter;}
        else
          {coloffset = ibigcenter - line;}
        }
      else
        {
        coloffset = 0;
        if (sin(angle) > 0)
          {rowoffset = line - ibigcenter;}
        else
          {rowoffset = ibigcenter - line;}
        }

      prcf->lineoffset[line] = coloffset + prcf->N * rowoffset;

      NCprojcoeff_first_and_last(&first, &last, colarray1, rowarray1,
                coloffset, rowoffset, prcf->N, scale, radius);
      prcf->linefirst1[line] = first;
      prcf->linelast1[line]  = last;

      NCprojcoeff_first_and_last(&first, &last, colarray2, rowarray2,
                coloffset, rowoffset, prcf->N, scale, radius);
      prcf->linefirst2[line] = first;
      prcf->linelast2[line]  = last;

      prcf->linelast1[line]++; /* not really the last pixel but the last+1 */
      prcf->linelast2[line]++;
      }    

    
    free(colarray1);
    free(colarray2);
    free(rowarray1);
    free(rowarray2);
    }

  /* Allocate the work buffers
   **********/
  prcf = ProjCoeffArray;
  prcf->worksino  = (float *) calloc(prcf->nrproj * prcf->N, sizeof(float));
  prcf->calcsino  = (float *) calloc(prcf->nrproj * prcf->N, sizeof(float));
  prcf->workrecon = (float *) calloc(prcf->N  * prcf->N, sizeof(float));
  prcf->backatten = (float *) calloc(prcf->N  * prcf->N, sizeof(float));
  prcf->backatten_ok = 0;

  return(ProjCoeffArray);
  }
