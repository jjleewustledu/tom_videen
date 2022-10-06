/* module NCmake_empty_imagev7 */

/*
 general information: 
 This module creates an empty V70 matrix PetVolume.
 */

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : jan 1998
  Modifications :
* endtopic History

******************************************************************************/
#include <math.h>  /*NOINC*/
#include <stdio.h>   /*NOINC*/
#include <ctype.h>   /*NOINC*/
#include <string.h>   /*NOINC*/
#include <sys/time.h>   /*NOINC*/
#include <sys/time.h>   /*NOINC*/
#include "matrix.h"   /*NOINC*/
#include "matrix_extra.h"   /*NOINC*/
#include "NChrplus.h"   /*NOINC*/

/*enddeclars*/

/****************************************************************************/
/*global*/
int NCmake_empty_imagev7(  /* only IeeeFloat or SunShort (see matrix.h) */
  MatrixFile      *imgfile,
  Image_subheader *img_sub,
  int              matnum 
  )
{
int           nrcols, nrrows, nplanes, nbytes, i;
int           nblocks;
int           blkno;
int           startblock;
int           zeroblocks;
int           remainingblocks;
int           success;
char          *zeros;
struct MatDir matdir;
struct MatDir dir_entry;

success = 0;

if ((imgfile->mhptr->file_type != PetVolume))
  {
  printf("NCmake_empty_imagev7: image file should be PetVolume\n");
  return(success);
  }

/* Compute number of bytes required.
***********************************/
nrcols  = img_sub->x_dimension;
nrrows  = img_sub->y_dimension;
nplanes = img_sub->z_dimension;
switch (img_sub->data_type)
  {
  case IeeeFloat : nbytes = sizeof(float);
                   break;
  case SunShort  : nbytes = sizeof(short int);
                   break;
  default:
    {
    printf("NCmake_empty_imagev7: data type should be Sun float or short\n");
    return(success);
    }
  }
nblocks = (nrcols * nrrows * nplanes * nbytes + 511) / 512;

/* See if the matrix is already there.
 * If not, we enter a new subheader.
 * Otherwise, we return with success = 0.
*****/
if (matrix_find(imgfile, matnum, &matdir) == ERROR)
  {
  blkno = mat_enter(imgfile->fptr, imgfile->mhptr, matnum, nblocks) ;
  dir_entry.matnum  = matnum ;
  dir_entry.strtblk = blkno ;
  dir_entry.endblk  = dir_entry.strtblk + nblocks - 1 ;
  dir_entry.matstat = 1 ;
  insert_mdir(dir_entry, imgfile->dirlist) ;
  matdir = dir_entry ;
  }
else
  {
  /* printf("NCmake_empty_imagev7: matrix already present!\n"); */
  return(success);
  }

/* write subheader
****************/
mat_write_image_subheader(imgfile->fptr, imgfile->mhptr, matdir.strtblk,
                           img_sub);

/* Write zeros to the disk to allocate the disk space.
******************************************************/
zeroblocks = 10;
startblock = matdir.strtblk+1;
zeros      = (char *) calloc(zeroblocks, 512);
for (i = 0; i < zeroblocks*512; i++)
  zeros[i] = 0;
for (remainingblocks = nblocks; remainingblocks >= zeroblocks; 
     remainingblocks -= zeroblocks)
  {
  write_matrix_data(imgfile->fptr, startblock, zeroblocks, zeros, 
                    img_sub->data_type);
  startblock += zeroblocks;
  }
if (remainingblocks > 0)
  {
  write_matrix_data(imgfile->fptr, startblock, remainingblocks, zeros, 
                    img_sub->data_type);
  }
success = 1;
free(zeros);
return(success);
}

