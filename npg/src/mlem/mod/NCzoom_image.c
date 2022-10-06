/* module NCzoom_image */

/*
 general information: 
 This module zooms or unzooms, rotates and/or translates an image.
 To zoom it, it applies linear interpolation.
 To unzoom it, it adds pixels.
 */

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : feb 1999.
  Modifications :
* endtopic History

******************************************************************************/
#define __NCzoom_image_module 1
#include "stdio.h"  /*NOINC*/
#include "math.h"  /*NOINC*/

/*enddeclars*/
#define round(value) \
     ((int) (value + 0.5))

/*****************************************************************************/
  /* This module adds pixel values to compute the value of a larger pixel.
   * This routine is only useful for zoom factors smaller than 1.
   * When applied with zoomfactors larger than 1, it applies nearest neighbor
   * interpolation, doing the computations inefficiently.
   ********/
float NCunzoom_2d(float *image,
                float  coord[],
                int    dim[],
                float  zoomfactor)
  {
  double floor();
  double ceil();
  float  weights[20][20];
  float  gewicht;
  float  totweight;
  float  halfwidth;
  float  col0, col1, row0, row1;
  float  value;
  int    curcol;
  int    currow;
  int    colpoints, rowpoints;
  int    col, row;
  int    lastcol, lastrow;


  halfwidth = 0.5 / zoomfactor;

  /* These are the edgepoints of the big pixel, in the
   * coordinates of the original small pixels image.
   ******/
  col0 = coord[0] - halfwidth;
  col1 = coord[0] + halfwidth;
  row0 = coord[1] - halfwidth;
  row1 = coord[1] + halfwidth;

  colpoints  = round(col1) - round(col0) + 1;
  rowpoints  = round(row1) - round(row0) + 1;
  lastcol = colpoints - 1;
  lastrow = rowpoints - 1;

  /* Compute the weights proportional to the overlap of the
   * big pixel and the original small pixels.
   ********/
  for (col = 0; col < colpoints; col++)
    for (row = 0; row < rowpoints; row++)
      weights[col][row] = 1.0;

  gewicht = 0.5 + round(col0) - col0;
  for (row = 0; row < rowpoints; row++)
    weights[0][row] *= gewicht;

  gewicht = 0.5 + round(row0) - row0;
  for (col = 0; col < colpoints; col++)
    weights[col][0] *= gewicht;

  gewicht = 0.5 + col1 - round(col1);
  for (row = 0; row < rowpoints; row++)
    weights[lastcol][row] *= gewicht;

  gewicht = 0.5 + row1 - round(row1);
  for (col = 0; col < colpoints; col++)
    weights[col][lastrow] *= gewicht;

  /* Scan all the pixels under the original big pixel,
   * and if they are in the image, weight them and add them
   *********/
  value     = 0.0;
  totweight = 0.0;

  currow = round(row0);
  for (row = 0; row < rowpoints; row++)
    {
    if ((currow >= 0) && (currow < dim[1]))
      {
      curcol = round(col0);
      for (col = 0; col < colpoints; col++)
        {
        if ((curcol >= 0) && (curcol < dim[0]))
          {
          value += image[currow * dim[0] + curcol] * weights[col][row];
          totweight += weights[col][row];
          }
        curcol++;
        }
      currow++;
      }
    }
  
  /*
  printf("(\n");
  for (row = 0; row < rowpoints; row++)
    {
    for (col = 0; col < colpoints; col++)
      printf("%7.3f ", weights[col][row]);
    printf("\n");
    }
  printf(")\n");
  */

  return(value / totweight);
  }
  
/****************************************************************************/
  /* This module applies linear interpolation, to be used for zoom factors
   * higher than 1. When applied with zoom factors smaller than 1, it applies
   * undersampling.
  *************/
float NClin_2d_interp(float *image, 
                       float coord[], 
                       int dim[])
{

  float weight[2];
  float grey_scale[4];
  int x_pos_int;
  int y_pos_int;
  double floor();

  /* Check first if point is not outside the original image */  
  if ((coord[0] < 0) || (coord[0] > (dim[0]-1)) ||
      (coord[1] < 0) || (coord[1] > (dim[1]-1)))
     return(0.0);
  else
  {
    x_pos_int = (int) floor(coord[0]);
    y_pos_int = (int) floor(coord[1]);
    weight[0] = coord[0] - x_pos_int;
    weight[1] = coord[1] - y_pos_int;
  
    grey_scale[0] = *(image + x_pos_int + (y_pos_int * dim[0]));

    if (x_pos_int < (dim[0]-1))
      grey_scale[1] = *(image + x_pos_int + 1 + (y_pos_int * dim[0]));
    else
      grey_scale[1] = 0.0;

    if (y_pos_int < (dim[1]-1))
      grey_scale[2] = *(image + x_pos_int + ((y_pos_int + 1) * dim[0]));
    else
      grey_scale[2] = 0.0;

    if ((x_pos_int < (dim[0]-1)) && (y_pos_int < (dim[1]-1)))
      grey_scale[3] = *(image + x_pos_int + 1 + ((y_pos_int + 1) * dim[0]));
    else
      grey_scale[3] = 0.0;

    grey_scale[0] = grey_scale[0] * (1 - weight[0]) +
                    grey_scale[1] * weight[0];
    grey_scale[1] = grey_scale[2] * (1 - weight[0]) +
                    grey_scale[3] * weight[0];
    grey_scale[0] = grey_scale[0] * (1 - weight[1]) +
                    grey_scale[1] * weight[1];
    return(grey_scale[0]);
  }
}

/*-------------------------------------------------------------------------*/
/*global*/
float NCzoom_image(
   float   *image_out,
   float   *image_in,
   float    pixel_size,
   int      dim_in_x,
   int      dim_in_y,
   int      dim_out_x,
   int      dim_out_y,
   float    orig_zoom,
   float    x_off,
   float    y_off,
   float    degrees,
   int      verbose)

  {
  float       zoom;

  float       x_shift;
  float       y_shift;
  float       angle;
  float       cosangle;
  float       sinangle;
  float       center_y_in;
  float       center_x_in;
  float       center_y_out;
  float       center_x_out;
  float       x;
  float       y;
  float       xrot;
  float       yrot;
  float       coord[2];
  float      *left;
  float      *pin, *pout;
   
  double      cos();
  double      sin();
 
  int         dim[2];
  int         x_pos;
  int         y_pos;
  int         copy = 0;


  zoom    =  orig_zoom * dim_out_x / dim_in_x;
  x_shift = -x_off / pixel_size;
  y_shift = -y_off / pixel_size;

  if ((zoom == 1) && (dim_in_x == dim_out_x) && 
      (x_shift == 0.) && (y_shift == 0.) && (degrees == 0.))
    copy = 1;     

  if (verbose)
    {
    printf("Effective zoom is %g: ", zoom);
    if (copy)
      {
      printf("No zoom, no rotation: straightforward copy.\n");
      }
    else
      {
      if (zoom >= 1) printf("Applying linear interpolation to zoom.\n");
      else printf("Averaging over pixels to unzoom.\n");
      }
    }

  if (!copy)
    {
    angle    = -degrees * M_PI/180;
    cosangle = cos(angle);
    sinangle = sin(angle);
    
    center_y_in  = (dim_in_y - 1.0) / 2.0 + y_shift;
    center_x_in  = (dim_in_x - 1.0) / 2.0 - x_shift;
    center_y_out = (dim_out_x - 1.0) / 2.0;
    center_x_out = (dim_out_y - 1.0) / 2.0;
    
    for (y_pos = 0; y_pos < dim_out_y; y_pos++)
      {
      for (x_pos = 0; x_pos < dim_out_x; x_pos++)
        {
        x = (x_pos - center_x_out);
        y = -(y_pos - center_y_out);
        xrot =  ( cosangle * x + sinangle * y) / zoom;
        yrot =  ( -sinangle * x + cosangle * y) / zoom;
        coord[0] = center_x_in + xrot;
        coord[1] = center_y_in - yrot;
        dim[0] = dim_in_x;
        dim[1] = dim_in_y;
        
        /* If zoom >= 1, then use linear interpolation.
         * Otherwise, use the routine NCunzoom_2d, which adds pixels
         * to produce a larger pixel.
         ********/
        if (zoom >= 1)
          *(image_out + x_pos + y_pos * dim_out_x)
            = NClin_2d_interp(image_in, coord, dim);
        else
          *(image_out + x_pos + y_pos * dim_out_x)
            = NCunzoom_2d(image_in, coord, dim, zoom);
        }
      }    
    }
  else
    {
    pin  = image_in;
    pout = image_out;
    for (y_pos = 0; y_pos < dim_out_x; y_pos++)
      for (x_pos = 0; x_pos < dim_out_y; x_pos++)
        *pout++ = *pin++;
    }
  return(zoom);
}
