/* module NCwrite_imagev7 */

/*
 general information:
 This module writes a single plane to a previously created PetVolume.
 */

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : jan 1998
  Modifications :
* endtopic History

******************************************************************************/
#include <math.h>				 /* NOINC */
#include <stdio.h>				 /* NOINC */
#include <stdlib.h>				 /* NOINC */
#include <ctype.h>				 /* NOINC */
#include <string.h>				 /* NOINC */
#include "matrix.h"				 /* NOINC */
#include "matrix_extra.h"		 /* NOINC */
#include "NChrplus.h"			 /* NOINC */

/*enddeclars*/

/****************************************************************************/
/*global*/
int    NCwrite_imagev7 (		 /* only IeeeFloat or SunShort (see matrix.h) */
		                 MatrixFile * imgfile,
		                 Image_subheader * img_sub,	/* completely filled by the calle */
		                 void *data,	/* pointer to the data of type *img_sub->datatype */
		                 int matnum,	/* plane set to 1, further selected with plane */
		                 int plane		/* first plane is 1 !!! */
)
{
	struct MatDir   matdir;
	struct MatDir   dir_entry;
	int             nrcols, nrrows, nplanes, nbytes;
	int             planesize;
	int             file_pos;
	int             success;
	struct Matval   matval;

	success = 0;

/* If the matrix is not yet there, put a complete empty matrix in place. */

	if (matrix_find (imgfile, matnum, &matdir) == ERROR) {
		if (NCmake_empty_imagev7 (imgfile, img_sub, matnum) == 0) {
			printf ("NCwrite_imagev7: error creating empty PetVolume\n");
			return (success);
		}
	}
	nrcols = img_sub->x_dimension;
	nrrows = img_sub->y_dimension;
	nplanes = img_sub->z_dimension;
	switch (img_sub->data_type) {
	case IeeeFloat:
		nbytes = sizeof (float);
		break;
	case SunShort:
		nbytes = sizeof (short int);
		break;
	default:
		{
			printf ("NCwrite_imagev7: data type should be Sun float or short\n");
			return (success);
		}
	}

/* Adapt the main header	*/
  
	mat_numdoc (matnum, &matval);
	if (plane > imgfile->mhptr->num_planes)
		imgfile->mhptr->num_planes = plane;
	if (matval.frame > imgfile->mhptr->num_frames)
		imgfile->mhptr->num_frames = matval.frame;
	if (matval.gate > imgfile->mhptr->num_gates)
		imgfile->mhptr->num_gates = matval.gate;
	if (matval.bed > imgfile->mhptr->num_bed_pos)
		imgfile->mhptr->num_bed_pos = matval.bed;

	if (matrix_find (imgfile, matnum, &matdir) == ERROR) {
		printf ("NCwrite_imagev7: matrix not present in the file\n");
		return (success);
	}

	/* if data == NULL: write only the subheader */
	if (data == NULL) {
		mat_write_main_header (imgfile->fptr, imgfile->mhptr);
		mat_write_image_subheader (imgfile->fptr, imgfile->mhptr, matdir.strtblk, img_sub);
		success = 1;
		return (success);
	}
	planesize = nrcols * nrrows * nbytes;
	file_pos = matdir.strtblk * MatBLKSIZE;
	file_pos += (plane - 1) * planesize;

	if (fseek (imgfile->fptr, file_pos, 0) == -1) {
		printf ("NCwrite_imagev7: Error finding position in the file %s\n", imgfile->fname);
		return (success);
	}
	if (fwrite (data, planesize, 1, imgfile->fptr) != 1) {
		printf ("NCwrite_imagev7: Error writing to %s for plane %d\n", imgfile->fname, plane);
		return (success);
	}
	success = 1;
	return (success);
}
