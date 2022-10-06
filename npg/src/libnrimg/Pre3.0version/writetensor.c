/* =============================================================================
 *	Function:		writetensor
 *  Date:           Dec-95
 *  Author:         Tom Videen
 *	Description:	Write one frame of an image into a Numerical Recipes 3-D tensor.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libnrimg/Pre3.0version/RCS/writetensor.c,v 2.7 2003/07/01 19:16:56 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>
#include <nrutil.h>

int             writetensor (file, indx, image)
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

	if (fformat == MATRIX_REV7) {
		size = dim1 * dim2 * dim3;
		if ((img = (float *) malloc (size * sizeof (float))) == NULL) {
			fprintf (stderr, "ERROR [writetensor]: malloc img for writing %s\n", fname);
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
		fill_mh7 (file);
		fill_sh7 (file);
		if (wrimg7 (file, indx, img) == ERROR) {
			fprintf (stderr, "ERROR [writetensor]: writing frame %d of %s\n", indx->frm, fname);
			free (img);
			return (ERROR);
		}
		free (img);
		return (OK);

	} else if (fformat == MATRIX_REV6) {
		size = dim1 * dim2;
		if ((img = (float *) malloc (size * sizeof (float))) == NULL) {
			fprintf (stderr, "ERROR [writetensor]: malloc img for writing %s\n", fname);
			return (ERROR);
		}
		fill_mh6 (file);
		fill_sh6 (file);
		for (pln = 1; pln <= dim3; pln++) {
			indx->pln = pln;
			ix = 0;
			for (j = 1; j <= dim2; j++) {
				for (i = 1; i <= dim1; i++) {
					img[ix] = image[pln][j][i];
					ix++;
				}
			}
			if (wrimg6 (file, indx, img) == ERROR) {
				fprintf (stderr, "ERROR [writetensor]: writing frame %d plane %d of %s\n",
					 indx->frm, (int) pln, fname);
				free (img);
				return (ERROR);
			}
		}
		free (img);
		return (OK);

	} else if (fformat == PETT6) {
		size = dim1 * dim2;
		if ((img = (float *) malloc (size * sizeof (float))) == NULL) {
			fprintf (stderr, "ERROR [writetensor]: malloc img for writing %s\n", fname);
			return (ERROR);
		}
		fill_hp6 (file, indx);
		for (pln = 1; pln <= dim3; pln++) {
			indx->pln = pln;
			ix = 0;
			for (j = 1; j <= dim2; j++) {
				for (i = 1; i <= dim1; i++) {
					img[ix] = image[pln][j][i];
					ix++;
				}
			}
			if (wrimgp6 (file, img) == ERROR) {
				fprintf (stderr, "ERROR [writetensor]: writing frame %d plane %d of %s\n",
					 indx->frm, (int) pln, fname);
				free (img);
				return (ERROR);
			}
		}
		free (img);
		return (OK);

	} else if (fformat == INTERFILE) {
		if (open_if (file) == ERROR) {
			fprintf (stderr, "ERROR [writetensor]: cannot open %s\n", fname);
			return (ERROR);
		}
		if (wrimgif (file, image) == ERROR) {
			fprintf (stderr, "ERROR [writetensor]: writing frame %d of %s\n",
				 indx->frm, fname);
			return (ERROR);
		}
		return (OK);

	} else
		return (ERROR);
}
