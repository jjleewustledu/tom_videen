/* module NCmatrix_write_plane */

/*
 general information:
 This module a single plane to a matrix file, version 6 or 70.
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
#include <ctype.h>				 /* NOINC */
#include <string.h>				 /* NOINC */
#include <sys/time.h>				 /* NOINC */
#include <sys/time.h>				 /* NOINC */
#include "matrix.h"				 /* NOINC */
#include "matrix_extra.h"			 /* NOINC */
#include "NChrplus.h"				 /* NOINC */

/*enddeclars*/

/****************************************************************************/
/*global*/
int             NCmatrix_write_plane (
				                      MatrixFile * imgfile,
				                      MatrixData * imgmatrix,
				                      int plane,
				                      int frame,
				                      int gate,
				                      int bed)
{
	MatrixData     *slice;
	Image_subheader *imgsub;
	Attn_subheader *atnsub;
	struct Matval   val;
	int             i, npixels, nblks, s_matnum;
	int             matnum;

	switch (imgmatrix->mat_type) {
	case PetImage:
	case PetVolume:
		if (imgfile->mhptr->sw_version <= 6) {
			matnum = mat_numcod (frame, plane, gate, 0, bed);
			matrix_write (imgfile, matnum, imgmatrix);
			return (1);
		}
		imgsub = (Image_subheader *) imgmatrix->shptr;
		matnum = mat_numcod (frame, 1, gate, 0, bed);

		if (NCwrite_imagev7 (imgfile, imgsub, imgmatrix->data_ptr, matnum, plane))
			return (NCwrite_imagev7 (imgfile, imgsub, NULL, matnum, plane));
		break;

	case AttenCor:
		if (imgfile->mhptr->sw_version <= 6) {
			matnum = mat_numcod (frame, plane, gate, 0, bed);
			matrix_write (imgfile, matnum, imgmatrix);
			return (1);
		}
		atnsub = (Attn_subheader *) imgmatrix->shptr;
		matnum = mat_numcod (frame, 1, gate, 0, bed);

		if (NCwrite_attenv7 (imgfile, atnsub, (float *) (imgmatrix->data_ptr),
				     matnum, plane, 0))
			return (NCwrite_attenv7 (imgfile, atnsub, NULL, matnum, plane, 0));
		break;

	default:
		printf ("Illegal file type, only PetImage and AttenCor allowed\n");
		return (0);
		break;
	}

}
