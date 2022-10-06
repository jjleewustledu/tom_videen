/* module NCproj_clr */

/*
 general information: 
 This module sets for every detector line the first and last pixel in the
 image to zero. This is necessary if the field of view is limited to a circle.
 If not done, ML-EM uses the edge points of the FOV, which are seen in only
 a limited number of sinograms (because of interpolation effects) to
 introduce hot spots, explaining hot noise spots in the sinograms.
 */

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : mar 1996
  Modifications :
* endtopic History

******************************************************************************/
#define __NCproj_clr_module 1
#include "NCproj_bproj.h"
#include "math.h"

/*enddeclars*/

/*-------------------------------------------------------------------------*/
/*global*/
void NCproj_clrfov(
  float       *img,
  int          nrows,
  int          ncols,
  float        value
  )

  {
  int   col, row, pix;


  /* Scan all rows.
   * For every row, locate first nonzero pixel and set it to the value.
   ******/
  for (row = 0; row < nrows; row++)
    {
    pix = row * nrows;
    col = 0;
    while ((img[pix] == 0) && (col < ncols))
      {
      pix++;
      col++;
      }
    if (col < ncols)
      img[pix] = value;

    pix = row * nrows + ncols-1;
    col = ncols-1;
    while ((img[pix] == 0) && (col >= 0))
      {
      pix--;
      col--;
      }
    if (col >= 0)
      img[pix] = value;
    }

  /* Scan all cols.
   * For every col, locate first nonzero pixel and set it to the value.
   ******/
  for (col = 0; col < ncols; col++)
    {
    pix = col;
    row = 0;
    while ((img[pix] == 0) && (row < nrows))
      {
      pix += nrows;
      row++;
      }
    if (row < nrows)
      img[pix] = value;

    pix = col + (nrows-1) * nrows;
    row = nrows-1;
    while ((img[pix] == 0) && (row >= 0))
      {
      pix -= nrows;
      row--;
      }
    if (row >= 0)
      img[pix] = value;
    }
  }
