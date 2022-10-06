/* =============================================================================
 *	Function:		wrtensor
 *  Date:           Apr-96
 *  Author:         Tom Videen
 *	Description:	Write one frame of an image in a Numerical Recipes 3-D tensor
 *					to an output file which is a version 7 matrix file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libnrimg/Pre3.0version/RCS/wrtensor.c,v 2.5 1999/12/13 18:21:08 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>
#include <nrutil.h>

int             wrtensor (file, indx, image)
	ImageFile      *file;
	Index          *indx;
	float        ***image;
{
	char           *fname;
	float          *img;
	int             fformat;
	long            size;
	long            dim1, dim2, dim3;
	long            pln, ix, i, j;

	fname = file->name;
	fformat = file->mh->file_format;
	dim1 = (long) file->sh->dim1;
	dim2 = (long) file->sh->dim2;
	dim3 = (long) file->sh->dim3;

	if ((open_m7 (file)) == ERROR) {
		fprintf (stderr, "ERROR [wrtensor]: Cannot open %s for writing\n", fname);
		return (ERROR);
	}
	if (fformat != MATRIX_REV7) {
		fill_mh7 (file);
		fill_sh7 (file);
		file->mh->file_format = MATRIX_REV7;
	} else {
		update_mh7 (file);
		update_sh7 (file);
		file->m7->sh->z_dimension = dim3;
	}
	if (put_mh (file) == ERROR) {
		fprintf (stderr, "ERROR [wrtensor]: Cannot write MainHeader to %s\n", fname);
		return (ERROR);
	}
	size = dim1 * dim2 * dim3;
	if ((img = (float *) malloc (size * sizeof (float))) == NULL) {
		fprintf (stderr, "ERROR [wrtensor]: Cannot malloc img for writing %s\n", fname);
		return (ERROR);
	}
	ix = 0;
	for (pln = 1; pln <= dim3; pln++) {
		for (j = 1; j <= dim2; j++) {
			for (i = 1; i <= dim1; i++) {
				img[ix] = image[pln][j][i];
				ix++;
			}
		}
	}
	indx->pln = 1;
	file->m7->mh->file_type = (short) PetVolume;
	if (wrimg7 (file, indx, img) == ERROR) {
		fprintf (stderr, "ERROR [wrtensor]: Cannot write frame %d of %s\n",
			 indx->frm, fname);
		free (img);
		return (ERROR);
	}
	free (img);
	close_imgfile (file);
	return (OK);
}
