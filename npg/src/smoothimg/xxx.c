/*	$Log$ */
/* ===============================================================================
 *	Module:			smoothimg.c
 *	Date:			Sept 2012
 *	Author:			Tom Videen
 *	Description:	Average local pixels
 * ===============================================================================
 */
static char     rcsid[] = "$Header: /home/npggw/tom/src/smoothimg/RCS/smoothimg.c,v 2.0 2004/02/16 15:27:59 tom Exp $";

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <image.h>
#include <imgutil.h>
#include <libimage.h>
#include <libnrimg.h>
#include <nrutil.h>
#include <pettable.h>

main (argc, argv)
/*@unused@*/
	int             argc;
	char           *argv[];
{
	ImageFile      *file;
	Index           indx;
	MainHeader     *mh;
	SubHeader      *sh;
	FILE           *recfp;			 /* rec file pointer */
	char           *recfile;		 /* rec filename */
	void            rlft3 ();		 /* FFT transform */
	float        ***img;			 /* image array and frequency spectrum */
	float        ***data;			 /* padded, resampled array for FFT */
	float         **spec;			 /* contains portion of frequency spectrum not in */
	char           *fname1;			 /* file to process */
	char           *fname2;			 /* file to write */
	int             frm;			 /* frame number to process */
	char            runstamp[40];	 /* version, date and userid info */
	float           fac;			 /* filter factor */
	unsigned long   dim1, dim2, dim3; /* original array dimensions */
	unsigned long   nx, ny;			 /* array dimensions for FFT */
	float           dx, dy;			 /* delta frequency for each dimension */
	float           fx, fy;			 /* frequency in each dimension */
	float           f;				 /* square of radial frequency */
	float           fnorm;			 /* required to normalize inverse FFT */
	long            i, j, k;

/*	Get command line arguments	*/

	fname1 = argv[1];
	fname2 = argv[2];
	recfile = argv[4];
	(void) sprintf (runstamp, "%s %s %s %s %s %s", argv[5], argv[6], argv[7],
			argv[9], argv[10], argv[11]);

/*	Open rec file */

	recfp = fopen (recfile, "a");
	if (recfp == (FILE *) NULL) {
		fprintf (stderr, "ERROR [smoothimg]: Cannot open %s\n", recfile);
		return (ERROR);
	}

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
		fprintf (stderr, "ERROR [smoothimg]: Could not open %s\n", fname1);
		return (ERROR);
	}
	if (get_mh (file) == ERROR) {
		fprintf (stderr, "ERROR [smoothimg]: Could not get MainHeader from %s\n", fname1);
		return (ERROR);
	}
	if (existimgfrm (file, &indx) == ERROR) {
		if (frm == 1) {
			indx.frm = 2;
			if (existimgfrm (file, &indx) == ERROR) {
				indx.frm = 3;
				if (existimgfrm (file, &indx) == ERROR) {
					fprintf (stderr, "ERROR [smoothimg]: cannot find %s frame 1, 2 or 3\n", fname1);
					return (ERROR);
				}
			}
		} else {
			fprintf (stderr, "ERROR [smoothimg]: cannot find %s frame %d\n",
				 fname1, frm);
			return (ERROR);
		}
	}
	if (get_sh (file, &indx) == ERROR) {
		fprintf (stderr, "ERROR [smoothimg]: Could not read file subheader in %s\n", fname1);
		return (ERROR);
	}
	mh = file->mh;
	sh = file->sh;

	(void) save_rcs (file, rcsid, runstamp);

	dim1 = (unsigned long) sh->dim1;
	dim2 = (unsigned long) sh->dim2;
	dim3 = (unsigned long) sh->dim3;
	if (dim1 <= 128)
		nx = 128;
	else
		nx = 256;
	if (dim2 <= 128)
		ny = 128;
	else
		ny = 256;
	dx = 1.0 / ((float) nx * (sh->sample_size_1));
	dy = 1.0 / ((float) ny * (sh->sample_size_2));
	spec = matrix (1, 1, 1, 2 * ny);

/*	Read image */

	fprintf (stdout, "Filtering %s, frame %d\n", fname1, indx.frm);
	if ((img = readtensor (file, &indx)) == NULL)
		return (ERROR);
	recout (recfp, fname1);

/*	Pad before FFT */

	data = f3tensor (1, 1, 1, dim2, 1, dim1);
	fnorm = 2.0 / ((float) dim1 * (float) dim2);

/*	Order of elements in freq spectrum defined on pp 526-7 of Numerical Recipes in C */

	fc = -log (2.0) / pow (f0, 2.0);
	for (i = 1; i <= dim3; i++) {
		for (j = 1; j <= dim2; j++)
			for (k = 1; k <= dim1; k++)
				data[1][j][k] = img[i][j][k];
		data = padtensor (data, dim1, dim2, 1, nx, ny, 1);
		rlft3 (data, spec, 1, ny, nx, 1);/* wants dimensions z,y,x */
		for (j = 1; j <= ny; j++) {
			if (j <= ny / 2 + 1)
				fy = (j - 1) * dy;
			else
				fy = (ny - j + 1) * dy;
			for (k = 1; k <= nx / 2; k++) {
				fx = (k - 1) * dx;
				f = pow (fx, 2.0) + pow (fy, 2.0);
				fac = exp (f * fc);
				data[1][j][2 * k - 1] *= fac;
				data[1][j][2 * k] *= fac;
			}
			fx = (nx / 2) * dx;
			f = pow (fx, 2.0) + pow (fy, 2.0);
			fac = exp (f * fc);
			spec[1][2 * j - 1] *= fac;	/* effectively 0 */
			spec[1][2 * j] *= fac;
		}
		rlft3 (data, spec, 1, ny, nx, -1);
		data = trimtensor (data, nx, ny, 1, dim1, dim2, 1);

		for (j = 1; j <= dim2; j++)
			for (k = 1; k <= dim1; k++)
				img[i][j][k] = fnorm * data[1][j][k];
	}

/*	Write image.
 *	The output frame number is set to 1.
 *	The subheader is altered to add a constant (GAUSSIAN_FILTER_CODE)
 *	to the reconstruction filter_code, and the order is inserted
 *	in another filter parameter which was assumed to be unimportant.
 */

	indx.frm = 1;
	fprintf (stdout, "Writing filtered image %s, frame %d\n", fname2, indx.frm);
	close_imgfile (file);
	(void) strcpy (file->name, fname2);
	(void) strcpy (file->mode, "w");
	if (open_imgfile (file) == ERROR) {
		fprintf (stderr, "ERROR [smoothimg]: Could not open %s\n", fname2);
		return (ERROR);
	}
	mh->nframe = indx.frm;
	mh->pr_filter = 7;
	sh->filter_code += GAUSSIAN_FILTER_CODE;
	sh->filter_ramp_slope = f0;
	if (put_mh (file) == ERROR)
		return (ERROR);
	if (writetensor (file, &indx, img) == ERROR)
		return (ERROR);
	free_matrix (spec, 1, 1, 1, 2 * ny);
	free_f3tensor (img, 1, dim3, 1, dim2, 1, dim1);
	free_imgfile (file);
	(void) fclose (recfp);
	return (OK);
}
