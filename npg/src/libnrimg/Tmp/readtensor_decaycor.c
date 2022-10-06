/* =============================================================================
 *	Function:		readtensor_decaycor
 *  Date:           Aug 2008
 *  Author:         Tom Videen
 *  Description:    Read one frame of an image into a Numerical Recipes 3-D tensor
 *					and scale it appropriately.
 * =============================================================================
 */
/*@unused@*/
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libnrimg/RCS/readtensor.c,v 3.1 2004/02/20 16:12:26 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>
#include <nrutil.h>

float        ***readtensor_decaycor (file, indx)
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
		fprintf (stderr, "ERROR [readtensor_decaycor]: Cannot malloc image for %s\n", fname);
		return (NULL);
	}
	for (pln = 1; pln <= dim3; pln++) {
		indx->pln = pln;
		if ((img = rdimg_decaycor (file, indx)) == NULL) {
			fprintf (stderr, "ERROR [readtensor_decaycor]: Cannot read frame %d plane %d of %s\n",
				 indx->frm, (int) pln, fname);
			free_f3tensor (image, 1, dim3, 1, dim2, 1, dim1);
			free (image);
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
