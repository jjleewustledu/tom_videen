/* module NCwrite_attenv7 */

/*
 general information: 
  Writes the data to the file, created by NCmake_empty_attenv7.
  When data == NULL, the subheader is written to the file. Otherwise,
  only the data are written.

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

/****************************************************************************/
/*global*/
int NCwrite_attenv7(       /* only IeeeFloat (see matrix.h) */
  MatrixFile      *attenfile,
  Attn_subheader  *atten_sub,  /* completely filled by the caller! */
  float           *data,   /* pointer to the data of type img_sub->datatype*/
  int              matnum,  /* plane set to 1, further selected with plane */
  int              plane,
  int              segment
  )

{
int              i;
int              success;
int              nrangle;
int              nrdet;
int              file_pos;
int              nrplane;
int              sinosize;
int              linesize;
int              viewsize;
int              group;
float           *lineptr;
struct MatDir    matdir;

success = 0;

/* If the matrix is not yet there, put a complete empty matrix in place.
***************************/
if (matrix_find(attenfile, matnum, &matdir) == ERROR)
  {
  if (NCmake_empty_attenv7(attenfile, atten_sub, matnum) == 0)
    {
    printf("NCwrite_attenv7: error creating empty Attn Corr File\n");
    return(success);
    }
  if (matrix_find(attenfile, matnum, &matdir) == ERROR)
    {
    printf("NCwrite_attenv7: failed to create empty Attn Corr File\n");
    return(success);
    }
  }

if (data == NULL) /* data == NULL: write only the subheader! */
  {
  mat_write_attn_subheader(attenfile->fptr, attenfile->mhptr, matdir.strtblk,
                           atten_sub);
  success = 1;
  return(success);
  }

group     = abs(segment);
nrangle   = atten_sub->num_angles;
nrdet     = atten_sub->num_r_elements;
linesize  = nrdet * sizeof(float);
sinosize  = nrangle * linesize;
nrplane   = atten_sub->z_elements[group];
if (group != 0) nrplane /= 2;
viewsize  = nrplane * linesize;

/* Set file_pos to begin, and skip groups until group with current
 * segment is reached.
 *********/
file_pos = (matdir.strtblk) * MatBLKSIZE;
for (i = 0; i < group; i++)
  file_pos += atten_sub->z_elements[i] * sinosize;
if (segment < 0)
  file_pos += nrplane * sinosize;

if (atten_sub->storage_order == 1)  /* sinogram mode */
  {
  file_pos += (plane-1) * sinosize;
  
  if (fseek(attenfile->fptr, file_pos, 0) == -1)
    {
    printf("Error finding position in the file %s\n", attenfile->fname);
    return(success);
    }
  if (fwrite(data, sinosize, 1, attenfile->fptr) != 1)
    {
    printf("Error writing to %s for plane %d, segment %d\n", attenfile->fname,
           plane, segment);
    return(success);
    }
  success = 1;
  }
else /* view mode */
  {
  file_pos += (plane-1) * linesize;
  lineptr  = data;
  for (i = 0; i < nrangle; i++)
    {
    if (fseek(attenfile->fptr, file_pos, 0) == -1)
      {
      printf("Error finding position in the file %s\n", attenfile->fname);
      return(success);
      }
    if (fwrite(lineptr, linesize, 1, attenfile->fptr) != 1)
      {
      printf("Error writing to %s for plane %d, segment %d\n", 
             attenfile->fname, plane, segment);
      return(success);
      }
    file_pos += viewsize;
    lineptr  += nrdet;
    }
  success = 1;
  }
return(success);
}

