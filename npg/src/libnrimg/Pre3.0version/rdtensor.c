/* =============================================================================
 *	Function:		rdtensor
 *  Date:           Apr-96
 *  Author:         Tom Videen
 *  Description:    Read one frame of an image into a Numerical Recipes 3-D tensor
 *					and scale it appropriately.
 *					Expand input array dimension to "odim" (for PETT VI)
 * =============================================================================
 */

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libnrimg/Pre3.0version/RCS/rdtensor.c,v 2.8 2003/12/23 17:45:49 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>
#include <nrutil.h>

float        ***rdtensor (fname, indx, odim)
	char           *fname;			 /* filename of image to read */
	Index          *indx;			 /* frame, plane, gate, data, bed */
	long           *odim;			 /* output image dimensions */
{
	ImageFile      *file;
	Index           indx2;
	float        ***image;
	float          *img;
	int             reverse;
	long            idim[3];		 /* input image dimensions */
	long            i0, j0;			 /* offset to slice dimensions on input */
	long            i, j, ix, pln;		 /* indices in loops */

	file = make_imgfile (fname);
	if (open_imgfile (file) == ERROR) {
		fprintf (stderr, "ERROR [rdtensor]: Cannot open %s\n", fname);
		return (NULL);
	}
	if (get_mh (file) == ERROR) {
		fprintf (stderr, "ERROR [rdtensor]: Cannot get MainHeader from %s\n", fname);
		return (NULL);
	}
	indx2.frm = indx->frm;
	indx2.pln = indx->pln;
	indx2.gate = indx->gate;
	indx2.data = indx->data;
	indx2.bed = indx->bed;
	if (existimgfrm (file, indx2) == ERROR) {
		indx2.frm = firstimgfrm (file);
		if (existimgfrm (file, indx2) == ERROR) {
			fprintf (stderr, "ERROR [rdtensor]: Cannot read frame %d from %s\n", indx2.frm, fname);
			return (NULL);
		}
	}
	if (get_sh (file, indx2) == ERROR) {
		fprintf (stderr, "ERROR [rdtensor]: Cannot read file subheader in %s\n", fname);
		return (NULL);
	}
	reverse = file->mh->orientation;
	idim[0] = (long) file->sh->dim1;
	idim[1] = (long) file->sh->dim2;
	idim[2] = (long) file->sh->dim3;
	i0 = (odim[0] - idim[0]) / 2;
	j0 = (odim[1] - idim[1]) / 2;

	if ((image = f3tensor (1, idim[2], 1, odim[1], 1, odim[0])) == NULL) {
		fprintf (stderr, "ERROR [rdtensor]: Cannot malloc image for %s\n", fname);
		return (NULL);
	}
	for (pln = 1; pln <= idim[2]; pln++) {
		for (j = 1; j <= odim[1]; j++) {
			for (i = 1; i <= odim[0]; i++) {
				image[pln][j][i] = 0.;
			}
		}
	}

	for (pln = 1; pln <= idim[2]; pln++) {
		indx2.pln = pln;
		if ((img = rdimg (file, &indx2)) == NULL) {
			fprintf (stderr, "ERROR [rdtensor]: Cannot read frame %d plane %d of %s\n",
				 indx2.frm, (int) pln, fname);
			return (NULL);
		}
		if (reverse != 0)
			img = flipimg (file, img);
		ix = 0;
		for (j = 1; j <= idim[1]; j++) {
			for (i = 1; i <= idim[0]; i++) {
				image[pln][j + j0][i + i0] = img[ix];
				ix++;
			}
		}
		free (img);
	}
	if (reverse != 0)
		file->mh->orientation = 0;
	return (image);
}
