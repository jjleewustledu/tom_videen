/* =============================================================================
 *	Function:		readtensor7
 *  Date:           Feb-96
 *  Author:         Tom Videen
 *  Description:    Read one frame of an image into a Numerical Recipes 3-D tensor
 *					and scale it appropriately.
 *					Expand input array dimension to "odim" (for PETT VI)
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/petsun19/tom/src/libnrimg/RCS/readtensor7.c,v 1.1 1996/03/12 22:47:08 tom Exp tom $";
#endif

#include <stdio.h>
#include <libimg.h>
#include <nr.h>
#include <nrutil.h>

float        ***readtensor7 (filnam, frame, mh, sh, odim)
	char           *filnam;
	int             frame;
	MainHeader     *mh;
	SubHeader      *sh;
	long		   *odim;		/* output image dimensions */

{
	Main_header7   *mh7;
	Main_header    *mh6;
	float        ***image;
	float          *img;
	float           scaler;
	int             fformat;
	int             reverse;
	long            idim[3];		/* input image dimensions */
	long            i0, j0;			/* offset to slice dimensions on input */
	long            i, j, indx, pln;
	int             gate = 1, data = 0, bed = 0;

	fformat = mh->file_format;
	reverse = mh->orientation;
	idim[0] = (long) sh->dim1;
	idim[1] = (long) sh->dim2;
	idim[2] = (long) sh->dim3;
	i0 = (odim[0] - idim[0])/2;
	j0 = (odim[1] - idim[1])/2;
	if ((image = f3tensor (1, idim[2], 1, odim[1], 1, odim[0])) == NULL) {
		fprintf (stderr, "ERROR [readtensor7]: Cannot malloc image for %s\n", filnam);
		return (NULL);
	}

	if (fformat == MATRIX_REV7) {
		mh7 = mh_to_mh7 (mh);
		if (sh->duration == 0)		 /* Attenuation images have duration = 0 */
			scaler = ATNSCALER;
		else
			scaler = REV7SCALER * (float) sh->duration;
		for (pln = 1; pln <= idim[2]; pln++) {
			if ((img = rdimg7 (mh7, filnam, pln, frame, gate, data, bed)) == NULL) {
				fprintf (stderr, "ERROR [readimgfrm]: Cannot read frame %d plane %d of %s\n",
					 frame, pln, filnam);
				return (NULL);
			}
			if (reverse != 0)
				img = flipimg (img, mh, sh);
			indx = 0;
			for (j = 1; j <= idim[1]; j++) {
				for (i = 1; i <= idim[0]; i++) {
					image[pln][j+j0][i+i0] = scaler * img[indx];
					indx++;
				}
			}
			free (img);
		}
	} else if (fformat == MATRIX_REV6) {
		mh6 = mh_to_mh6 (mh);
		for (pln = 1; pln <= idim[2]; pln++) {
			if ((img = rdimg6 (mh6, filnam, pln, frame, gate, data, bed)) == NULL) {
				fprintf (stderr, "ERROR [readimgfrm]: Cannot read frame %d plane %d of %s\n",
					 frame, pln, filnam);
				return (NULL);
			}
			if (reverse != 0)
				img = flipimg (img, mh, sh);
			if (sh->duration == 0)		 /* Attenuation images have duration = 0 */
				scaler = ATNSCALER;
			else
				scaler = (float) sh->duration;
			indx = 0;
			for (j = 1; j <= idim[1]; j++) {
				for (i = 1; i <= idim[0]; i++) {
					image[pln][j+j0][i+i0] = scaler * img[indx];
					indx++;
				}
			}
			free (img);
		}
	} else if (fformat == PETT6) {
		for (pln = 1; pln <= idim[2]; pln++) {
			if ((img = rdimgp6 (filnam, pln)) == NULL) {
				fprintf (stderr, "ERROR [readimgfrm]: Cannot read frame %d plane %d of %s\n",
					 frame, pln, filnam);
				return (NULL);
			}
			indx = 0;
			for (j = 1; j <= idim[1]; j++) {
				for (i = 1; i <= idim[0]; i++) {
					image[pln][j+j0][i+i0] = img[indx];
					indx++;
				}
			}
			free (img);
		}
	} else if (fformat == INTERFILE) {
		for (pln = 1; pln <= idim[2]; pln++) {
			if ((img = rdimg_if (filnam, pln, frame)) == NULL) {
				fprintf (stderr, "ERROR [readimgfrm]: Cannot read frame %d plane %d of %s\n",
					 frame, pln, filnam);
				return (NULL);
			}
			if (reverse != 0)
				img = flipimg (img, mh, sh);
			indx = 0;
			for (j = 1; j <= idim[1]; j++) {
				for (i = 1; i <= idim[0]; i++) {
					image[pln][j+j0][i+i0] = img[indx];
					indx++;
				}
			}
			free (img);
		}
	} else
		return (NULL);

	return (image);
}
