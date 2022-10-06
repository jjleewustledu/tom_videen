/*	$Id$
	$Log$ */
/* ===============================================================================
 *	Module:			median.c
 *	Date:			Apr 2013
 *	Author:			Tom Videen
 *	Description:	Apply 2D 3x3 median filter to an image file.
 * ===============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>
#include <libimage.h>
#include <libnrimg.h>
#include <nrutil.h>
#include <pettable.h>

float           getmedian (int n, float x[])
{
	float           temp;
	int             i, j;
	for (i = 0; i < n - 1; i++) {
		for (j = 0; j < n - 1; j++) {
			if (x[j] > x[i]) {
				temp = x[i];
				x[i] = x[j];
				x[j] = temp;
			}
		}
	}
	return x[n / 2];
}

int             main (argc, argv)
/*@unused@*/
	int             argc;
	char           *argv[];
{
	ImageFile      *file;
	Index           indx;
	MainHeader     *mh;
	SubHeader      *sh;
	float        ***img;			 /* input image array */
	float        ***fmg;			 /* filtered outupt image array */
	char           *fname1;			 /* file to process */
	char           *fname2;			 /* file to write */
	int             frm;			 /* frame number to process */
	float           a[9];			 /* array from which to get median value */
	unsigned long   dim1, dim2, dim3;	 /* array dimensions */
	long            i, j, k, m, p, q;

/*	Get command line arguments	*/

	fname1 = argv[1];
	fname2 = argv[2];

/*	Get main header and subheader.
 *	Store RCS version in mh->sw_revision.
 */

	frm = 1;
	indx.frm = frm;
	indx.pln = 1;
	indx.gate = 1;
	indx.data = 0;
	indx.bed = 0;
	file = make_imgfile (fname1);
	if (open_imgfile (file) == ERROR) {
		fprintf (stderr, "ERROR [median]: opening %s\n", fname1);
		return (ERROR);
	}
	if (get_mh (file) == ERROR) {
		fprintf (stderr, "ERROR [median]: Reading MainHeader from %s\n", fname1);
		return (ERROR);
	}
	if (existimgfrm (file, &indx) == ERROR) {
		if (frm == 1) {
			indx.frm = 2;
			if (existimgfrm (file, &indx) == ERROR) {
				indx.frm = 3;
				if (existimgfrm (file, &indx) == ERROR) {
					fprintf (stderr, "ERROR [median]: cannot find %s frame 1, 2 or 3\n", fname1);
					return (ERROR);
				}
			}
		} else {
			fprintf (stderr, "ERROR [median]: cannot find %s frame %d\n", fname1, frm);
			return (ERROR);
		}
	}
	if (get_sh (file, &indx) == ERROR) {
		fprintf (stderr, "ERROR [median]: reading file subheader in %s\n", fname1);
		return (ERROR);
	}
	mh = file->mh;
	sh = file->sh;

	dim1 = (unsigned long) sh->dim1;
	dim2 = (unsigned long) sh->dim2;
	dim3 = (unsigned long) sh->dim3;

/*	Read image */

	fprintf (stdout, "Filtering %s, frame %d\n", fname1, indx.frm);
	if ((img = readtensor (file, &indx)) == NULL)
		return (ERROR);

/*	Create output image array */

	if ((fmg = readtensor (file, &indx)) == NULL)
		return (ERROR);

	for (i = 1; i <= dim3; i++)
		for (j = 2; j <= dim2 - 1; j++)
			for (k = 2; k <= dim1 - 1; k++) {
				m = 0;
				for (p = -1; p <= 1; p++)
					for (q = -1; q <= 1; q++) {
						a[m] = img[i][j + p][k + q];
						m++;
					}
				fmg[i][j][k] = getmedian (9, a);
			}

/*	Zero the edge pixels */

	for (i = 1; i <= dim3; i++) {
		for (j = 1; j <= dim2; j++) {
			fmg[i][j][1] = 0;
			fmg[i][j][dim1] = 0;
		}
		for (k = 1; k <= dim1; k++) {
			fmg[i][1][k] = 0;
			fmg[i][dim2][k] = 0;
		}
	}


/*	Write image.
 *	The output frame number is set to 1.
 */

	indx.frm = 1;
	fprintf (stdout, "Writing filtered image %s, frame %d\n", fname2, indx.frm);
	close_imgfile (file);
	(void) strcpy (file->name, fname2);
	(void) strcpy (file->mode, "w");
	if (open_imgfile (file) == ERROR) {
		fprintf (stderr, "ERROR [median]: opening %s\n", fname2);
		return (ERROR);
	}
	mh->nframe = indx.frm;
	mh->pr_filter = 7;
/*
	sh->filter_code += MEDIAN_FILTER_CODE;
*/
	if (put_mh (file) == ERROR)
		return (ERROR);
	if (writetensor (file, &indx, fmg) == ERROR)
		return (ERROR);
	free_f3tensor (img, 1, dim3, 1, dim2, 1, dim1);
	free_f3tensor (fmg, 1, dim3, 1, dim2, 1, dim1);
	free_imgfile (file);
	return (OK);
}
