/* =============================================================================
 *	Function:		readtensor
 *  Date:           Feb-96
 *  Author:         Tom Videen
 *  Description:    Read one frame of an image into a Numerical Recipes 3-D tensor
 *					and scale it appropriately.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libnrimg/Pre3.0version/RCS/readtensor.c,v 2.4 2003/12/23 17:45:49 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>
#include <nrutil.h>

float        ***readtensor (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	float        ***image;
	float          *img;
	char           *fname;
	int             reverse;
	long            dim1, dim2, dim3;
	long            i, j, ix, pln;

	fname = file->name;
	reverse = file->mh->orientation;
	dim1 = (long) file->sh->dim1;
	dim2 = (long) file->sh->dim2;
	dim3 = (long) file->sh->dim3;

	if ((image = f3tensor (1, dim3, 1, dim2, 1, dim1)) == NULL) {
		fprintf (stderr, "ERROR [readtensor]: Cannot malloc image for %s\n", fname);
		return (NULL);
	}
	for (pln = 1; pln <= dim3; pln++) {
		indx->pln = pln;
		if ((img = rdimg (file, indx)) == NULL) {
			fprintf (stderr, "ERROR [readtensor]: Cannot read frame %d plane %d of %s\n",
				 indx->frm, (int) pln, fname);
			return (NULL);
		}
		if (reverse != 0)
			img = flipimg (file, img);
		ix = 0;
		for (j = 1; j <= dim2; j++) {
			for (i = 1; i <= dim1; i++) {
				image[pln][j][i] = img[ix];
				ix++;
			}
		}
		free (img);
	}
	if (reverse != 0)
		file->mh->orientation = 0;
	return (image);
}
