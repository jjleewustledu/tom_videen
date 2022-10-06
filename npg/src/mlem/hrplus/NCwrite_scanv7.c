/* module NCwrite_scanv7 */

/*
 general information: 
  Writes the data to the file, created by NCmake_empty_scanv7.
  When data == NULL, the subheader is written to the file. Otherwise,
  only the data are written.

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
int NCwrite_scanv7(
  MatrixFile       *sinofile,    /* the file, already open */
  Scan3D_subheader *scan3d_sub,  /* subheader describing ENTIRE 4D scan.
                                    Only written when data == NULL. */
  short            *data,        /* the current sinogram, when NULL, the
                                    subheader is written*/
  int               matnum,      /* matnum of the subheader (plane = 1) */
  int               plane,       /* the plane number of the sinogram */
  int               segment      /* the segment of the sinogram */
  )
{
int              i;
int              success;
int              nrangle;
int              nrdet;
int              group;
int              file_pos;
int              nrplane;
int              sinosize;
int              linesize;
int              viewsize;
short int       *lineptr;
struct MatDir    matdir;

success = 0;

if (matrix_find(sinofile, matnum, &matdir) == ERROR)
  {
  printf("NCwrite_3dscan: matrix not present. ");
  printf("Call NCmake_empty_scanv7 first!\n");
  return(success);
  }

if (data == NULL) /* data == NULL: write only the subheader! */
  {
  mat_write_Scan3D_subheader(sinofile->fptr, sinofile->mhptr, matdir.strtblk,
                             scan3d_sub);
  success = 1;
  return(success);
  }

group     = abs(segment);
nrangle   = scan3d_sub->num_angles;
nrdet     = scan3d_sub->num_r_elements;
linesize  = nrdet * sizeof(short);
sinosize  = nrangle * linesize;
nrplane   = scan3d_sub->num_z_elements[group];
if (group != 0) nrplane /= 2;
viewsize  = nrplane * linesize;

/* Set file_pos to begin, and skip groups until group with current
 * segment is reached.
 *********/
file_pos = (matdir.strtblk + 1) * MatBLKSIZE;
for (i = 0; i < group; i++)
  file_pos += scan3d_sub->num_z_elements[i] * sinosize;
if (segment < 0)
  file_pos += nrplane * sinosize;

if (scan3d_sub->storage_order == 1)  /* sinogram mode */
  {
  file_pos += plane * sinosize;
  
  if (fseek(sinofile->fptr, file_pos, 0) == -1)
    {
    printf("Error finding position in the file %s\n", sinofile->fname);
    return(success);
    }
  if (fwrite(data, sinosize, 1, sinofile->fptr) != 1)
    {
    printf("Error writing to %s for plane %d, segment %d\n", sinofile->fname,
           plane, segment);
    return(success);
    }
  success = 1;
  }
else /* view mode */
  {
  file_pos += plane * linesize;
  lineptr  = data;
  for (i = 0; i < nrangle; i++)
    {
    if (fseek(sinofile->fptr, file_pos, 0) == -1)
      {
      printf("Error finding position in the file %s\n", sinofile->fname);
      return(success);
      }
    if (fwrite(lineptr, linesize, 1, sinofile->fptr) != 1)
      {
      printf("Error writing to %s for plane %d, segment %d\n", sinofile->fname,
             plane, segment);
      return(success);
      }
    file_pos += viewsize;
    lineptr  += nrdet;
    }
  success = 1;
  }
return(success);
}

