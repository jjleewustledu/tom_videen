/* module NCread_mat_float */

/*
 general information: 
 This module reads a matrix V70 file into a float image.
 */

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : jan 1998
  Modifications :
* endtopic History

******************************************************************************/
#include <math.h>  /*NOINC*/
#include <stdio.h>  /*NOINC*/
#include <ctype.h>  /*NOINC*/
#include <string.h>  /*NOINC*/
#include "matrix.h"  /*NOINC*/
#include "matrix_extra.h"  /*NOINC*/
#include "NChrplus.h"  /*NOINC*/

/*enddeclars*/

/****************************************************************************/
/*global*/
float* NCread_mat_float(MatrixData *matrix, 
                     float      scale)
  {
  float   *image;
  float   *imgptr;
  int      nrcols, nrrows, i;
  char    *byteptr;
  short   *shortptr;
  int     *longptr;
  float   *floatptr;
  float    globalscale;
  float    meanval;

  nrcols = matrix->xdim;
  nrrows = matrix->ydim;

  globalscale = matrix->scale_factor * scale;

  switch (matrix->data_type)
    {
    case ByteData:
    /* printf("bytes\n"); */
      image    = (float *) calloc(nrcols*nrrows, sizeof(float));
      imgptr = image;
      byteptr = (char *) matrix->data_ptr;
      for (i = 0; i < nrcols*nrrows; i++)
        *imgptr++ = *byteptr++ * globalscale;
      break;

    case VAX_Ix2:
    case SunShort:
    /* printf("words\n"); */
      image    = (float *) calloc(nrcols*nrrows, sizeof(float));
      imgptr = image;
      shortptr = (short *) matrix->data_ptr;
      for (i = 0; i < nrcols*nrrows; i++)
        *imgptr++ = *shortptr++ * globalscale;
      break;

    case VAX_Ix4:
    case SunLong:
    /* printf("long words\n"); */
      image    = (float *) calloc(nrcols*nrrows, sizeof(float));
      imgptr = image;
      longptr = (int *) matrix->data_ptr;
      for (i = 0; i < nrcols*nrrows; i++)
        *imgptr++ = *longptr++ * globalscale;
      break;

    case VAX_Rx4:
    case IeeeFloat:
      if (globalscale == 1.0)
        {
        /* printf("unscaled floats\n"); */
        image = (float *) matrix->data_ptr;
        }
      else
        {
        /* printf("scaled floats\n"); */
        image    = (float *) calloc(nrcols*nrrows, sizeof(float));
        imgptr = image;
        floatptr = (float *)  matrix->data_ptr;
        for (i = 0; i < nrcols*nrrows; i++)
          *imgptr++ = *floatptr++ * globalscale;
        }
      break;

    default:
      printf("Unexpected datatype, giving up (sorry)\n");
      exit(0);
    }

  return(image);
  }

