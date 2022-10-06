/* module NCmake_empty_scanv7 */

/*
 general information: 
   reserves empty disk space for a sino3D matrix.

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

/*======================================================================*/
int NCmake_empty_scanv7(
  MatrixFile       *sinofile, 
  Scan3D_subheader *scan3d_sub,
  int               matnum
  )

{
int    success;
int    nrdet, nrangle, nrseg, i_seg;
int    nrplane[64];
int    nbytes, nblocks;
int    blkno;
int    zeroblocks, remainingblocks;
int    startblock;
short *zeros;
struct MatDir matdir;
struct MatDir dir_entry;

success = 0;

/* Only works for Short3dSinogram files
*****************/
if ((sinofile->mhptr->file_type != Short3dSinogram) ||
    (scan3d_sub->data_type      != SunShort))
  {
  printf("NCmake_empty_scanv7: only for Short3dSinogram\n");
  return(success);
  }

/* Compute number of bytes required.
***********************************/
nrdet   = scan3d_sub->num_r_elements;
nrangle = scan3d_sub->num_angles;
nrseg = 0;
nbytes = 0;
for (i_seg =0; scan3d_sub->num_z_elements[i_seg] != 0; i_seg++) 
  {
  nrplane[i_seg] = scan3d_sub->num_z_elements[i_seg];
  nrseg++;
  nbytes += nrdet * nrangle * nrplane[i_seg] * sizeof(short);
  }
nblocks = (int) ceil(((float) nbytes) / 512.);
printf("Allocating %d blocks on disk\n", nblocks);

/* See if the matrix is already there.
 * If not, we enter a new subheader.
 * Otherwise, we return with success = 0.
*****/
if (matrix_find(sinofile, matnum, &matdir) == ERROR)
  {
  blkno = mat_enter(sinofile->fptr, sinofile->mhptr, matnum, nblocks) ;
  dir_entry.matnum = matnum ;
  dir_entry.strtblk = blkno ;
  dir_entry.endblk = dir_entry.strtblk + nblocks - 1 ;
  dir_entry.matstat = 1 ;
  insert_mdir(dir_entry, sinofile->dirlist) ;
  matdir = dir_entry ;
  }
else
  {
  /* printf("NCmake_empty_scanv7: matrix already present!\n"); */
  return(success);
  }

/* write subheader
****************/
mat_write_Scan3D_subheader(sinofile->fptr, sinofile->mhptr, matdir.strtblk,
                           scan3d_sub);
zeroblocks = 10;
startblock = matdir.strtblk+2;
zeros      = (short *) calloc(zeroblocks, 512);
for (remainingblocks = nblocks; remainingblocks >= zeroblocks; 
     remainingblocks -= zeroblocks)
  {
  write_matrix_data(sinofile->fptr, startblock, zeroblocks, zeros, 
                    scan3d_sub->data_type);
  startblock += zeroblocks;
  }
if (remainingblocks > 0)
  {
  write_matrix_data(sinofile->fptr, startblock, remainingblocks, zeros, 
                    scan3d_sub->data_type);
  }
success = 1;
free(zeros);
return(success);
}

