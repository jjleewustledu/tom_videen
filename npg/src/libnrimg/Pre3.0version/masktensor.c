/*  ================================================================================
 *  File Name:      masktensor.c
 *  Date:           Dec-95
 *  Author:         Tom Videen
 *  Description:    Create an image mask by first thresholding the input tensor
 *		and then setting all elements outside the outer boundary to 1 and all
 *		elements inside the boundary to 0
 *		The thresholded peak and the slices of the image which are templated
 *		are from firstpln to lastpln.
 *		Total pixels in the mask are returned in npixels.
 *	History:
 *		Algorithm from spideram: d2templa.f TOV Mar-87
 *
 *  =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libnrimg/Pre3.0version/RCS/masktensor.c,v 2.0 1996/04/24 23:00:22 tom Exp $";
#endif

#include <stdio.h>
#include <values.h>
#include <image.h>

#define NEITHER		0.
#define INSIDE		1.
#define OUTSIDE		2.

void            masktensor (file, img, mskpct, firstpln, lastpln, peakpln, npixels)
	ImageFile      *file;
	float        ***img;			 /* masked image (output) */
	int             mskpct;			 /* percent to mask by */
	int             firstpln;		 /* first slice to mask */
	int             lastpln;		 /* last slice to mask */
	int             peakpln;		 /* slice in which to find the peak */
	long           *npixels;		 /* total pixels within mask */
{
	int             more;
	float           threshold;
	float           peak;
	int             i, j, pln;
	long            dim1, dim2;

	dim1 = (long) file->sh->dim1;
	dim2 = (long) file->sh->dim2;

/*
 * Find the peak element and compute the absolute threshold
 */
	peak = MINFLOAT;
	if (peakpln > 0) {
		fprintf (stdout, "Peak from slice: %d\n", peakpln);
		for (j = 1; j <= dim2; j++) {
			for (i = 1; i <= dim1; i++) {
				if (img[peakpln][j][i] > peak)
					peak = img[peakpln][j][i];
			}
		}
	} else {
		fprintf (stdout, "Peak from slices: %d - %d\n", firstpln, lastpln);
		for (pln = firstpln; pln <= lastpln; pln++) {
			for (j = 1; j <= dim2; j++) {
				for (i = 1; i <= dim1; i++) {
					if (img[pln][j][i] > peak)
						peak = img[pln][j][i];
				}
			}
		}
	}
	threshold = peak * mskpct * 0.01;
	fprintf (stdout, "Peak = %9.3f  Threshold = %9.3f\n", peak, threshold);
	*npixels = 0;

/*
 *	Create a template with mskpct% of the whole brain maximum.
 *
 *	Assign INSIDE to all points >= threshold. Assign NEITHER to all points < threshold,
 */
	for (pln = firstpln; pln <= lastpln; pln++) {
		for (j = 1; j <= dim2; j++) {
			for (i = 1; i <= dim1; i++) {
				if (img[pln][j][i] > threshold)
					img[pln][j][i] = INSIDE;
				else
					img[pln][j][i] = NEITHER;
			}
		}
	}

/*
 *	Assign OUTSIDE to all points along the boundaries of each slice which are
 *	not INSIDE the template.
 */

	for (pln = firstpln; pln <= lastpln; pln++) {
		for (i = 1; i <= dim1; i++) {
			if (img[pln][1][i] != INSIDE)	/* top row */
				img[pln][1][i] = OUTSIDE;
			if (img[pln][dim2][i] != INSIDE)	/* bottom row */
				img[pln][dim2][i] = OUTSIDE;
		}
		for (j = 1; j <= dim2; j++) {
			if (img[pln][j][1] != INSIDE)	/* left row */
				img[pln][j][1] = OUTSIDE;
			if (img[pln][j][dim1] != INSIDE)	/* right row */
				img[pln][j][dim1] = OUTSIDE;
		}
	}

/*
 *	Now, starting with the boundaries of the array, expand the area excluded from
 *	the template by assigning OUTSIDE to all points directly adjacent to a point
 *	whose value is OUTSIDE and which are currently zero (neither INSIDE nor
 *	OUTSIDE).
 */
	more = FALSE;
	for (pln = firstpln; pln <= lastpln; pln++) {
		for (i = 2; i <= dim1 - 1; i++) {
			if (img[pln][1][i] == OUTSIDE && img[pln][2][i] < INSIDE) {	/* row below top row */
				img[pln][2][i] = OUTSIDE;
				more = TRUE;
			}
			if (img[pln][dim2][i] == OUTSIDE && img[pln][dim2 - 1][i] < INSIDE) {	/* row below bottom row */
				img[pln][dim2 - 1][i] = OUTSIDE;
				more = TRUE;
			}
		}
		for (j = 2; j <= dim2 - 1; j++) {
			if (img[pln][j][1] == OUTSIDE && img[pln][j][2] < INSIDE) {	/* col next to left */
				img[pln][j][2] = OUTSIDE;
				more = TRUE;
			}
			if (img[pln][j][dim1] == OUTSIDE && img[pln][j][dim1 - 1] < INSIDE) {	/* col next to right */
				img[pln][j][dim1 - 1] = OUTSIDE;
				more = TRUE;
			}
		}
	}

/*
 *	The following loop is repeated until it cycles with no more points added to the
 *	outside of the region.
 */

	while (more) {
		more = FALSE;
		for (pln = firstpln; pln <= lastpln; pln++) {
			for (i = 2; i <= dim1 - 1; i++) {
				for (j = 2; j <= dim2 - 1; j++) {
					if (img[pln][j][i] == OUTSIDE) {
						if (img[pln][j][i - 1] < INSIDE) {
							img[pln][j][i - 1] = OUTSIDE;
							more = TRUE;
						}
						if (img[pln][j][i + 1] < INSIDE) {
							img[pln][j][i + 1] = OUTSIDE;
							more = TRUE;
						}
						if (img[pln][j - 1][i] < INSIDE) {
							img[pln][j - 1][i] = OUTSIDE;
							more = TRUE;
						}
						if (img[pln][j + 1][i] < INSIDE) {
							img[pln][j + 1][i] = OUTSIDE;
							more = TRUE;
						}
					}
				}
			}
		}
	}

/*
 *	All points that are < OUTSIDE are assigned INSIDE. All points that are >=
 *	OUTSIDE are assigned 0.
 */
	for (pln = firstpln; pln <= lastpln; pln++) {
		for (j = 1; j <= dim2; j++) {
			for (i = 1; i <= dim1; i++) {
				if (img[pln][j][i] < OUTSIDE) {
					img[pln][j][i] = INSIDE;
					(*npixels)++;
				} else
					img[pln][j][i] = 0.;
			}
		}
	}
}
