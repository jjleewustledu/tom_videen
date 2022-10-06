/* module NCmake_empty_attenv7 */

/*
 general information: 
   reserves empty disk space for a atten matrix.

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : jan 1999
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
/*global*/
int NCmake_empty_attenv7(
  MatrixFile       *attenfile, 
  Attn_subheader   *atten_sub,
  int               matnum
  )

{
int    success;
int    nrdet, nrangle, nrseg, i_seg;
int    nrplane;
int    nbytes, nblocks;
int    blkno;
int    zeroblocks, remainingblocks;
int    startblock;
float *zeros;
struct MatDir matdir;
struct MatDir dir_entry;

success = 0;

/* Only works for float attencor files
*****************/
if ((attenfile->mhptr->file_type != AttenCor) ||
    (atten_sub->data_type      != IeeeFloat))
  {
  printf("NCmake_empty_attenv7: only for AttenCor with float pixel values\n");
  return(success);
  }

/* Compute number of bytes required.
***********************************/
nrdet   = atten_sub->num_r_elements;
nrangle = atten_sub->num_angles;
nrplane = atten_sub->num_z_elements;
nbytes = nrdet * nrangle * nrplane * sizeof(float);

nblocks = (int) ceil(((float) nbytes) / 512.);
/* printf("Allocating %d blocks on disk\n", nblocks); */

/* See if the matrix is already there.
 * If not, we enter a new subheader.
 * Otherwise, we return with success = 0.
*****/
if (matrix_find(attenfile, matnum, &matdir) == ERROR)
  {
  blkno = mat_enter(attenfile->fptr, attenfile->mhptr, matnum, nblocks) ;
  dir_entry.matnum = matnum ;
  dir_entry.strtblk = blkno ;
  dir_entry.endblk = dir_entry.strtblk + nblocks - 1 ;
  dir_entry.matstat = 1 ;
  insert_mdir(dir_entry, attenfile->dirlist) ;
  matdir = dir_entry ;
  }
else
  {
  /* printf("NCmake_empty_attenv7: matrix already present!\n"); */
  return(success);
  }

/* write subheader
****************/
mat_write_attn_subheader(attenfile->fptr, attenfile->mhptr, matdir.strtblk,
                          atten_sub);
zeroblocks = 10;
startblock = matdir.strtblk+1;
zeros      = (float *) calloc(zeroblocks, 512);
for (remainingblocks = nblocks; remainingblocks >= zeroblocks; 
     remainingblocks -= zeroblocks)
  {
  write_matrix_data(attenfile->fptr, startblock, zeroblocks, zeros, 
                    atten_sub->data_type);
  startblock += zeroblocks;
  }
if (remainingblocks > 0)
  {
  write_matrix_data(attenfile->fptr, startblock, remainingblocks, zeros, 
                    atten_sub->data_type);
  }
success = 1;
free(zeros);
return(success);
}
