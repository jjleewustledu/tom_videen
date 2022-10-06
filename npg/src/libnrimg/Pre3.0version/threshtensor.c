/*  ================================================================================
 *  File Name:      threshtensor.c
 *  Date:           Dec-96
 *  Author:         Tom Videen
 *  Description:    Create an image mask by first thresholding the input tensor
 *		setting all elements less than the threshold to 0
 *		Total pixels in the mask are returned in npixels.
 *  =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libnrimg/Pre3.0version/RCS/threshtensor.c,v 1.2 1998/12/09 16:01:23 tom Exp $";
#endif

#include <stdio.h>
#include <values.h>
#include <image.h>

void            threshtensor (file, img, mskpct, firstpln, lastpln, peakpln, npixels)
	ImageFile      *file;
	float        ***img;			 /* masked image (output) */
	int             mskpct;			 /* percent to mask by */
	int             firstpln;		 /* first slice to mask */
	int             lastpln;		 /* last slice to mask */
	int             peakpln;		 /* slice in which to find the peak */
	long           *npixels;		 /* total pixels within mask */
{
	float           threshold;
	float           peak;
	int             i, j, pln;
	long            dim1, dim2, dim3;

	dim1 = (long) file->sh->dim1;
	dim2 = (long) file->sh->dim2;
	dim3 = (long) file->sh->dim3;

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
	for (pln = 1; pln <= dim3; pln++) {
		for (j = 1; j <= dim2; j++) {
			for (i = 1; i <= dim1; i++) {
				if (img[pln][j][i] < threshold) {
					img[pln][j][i] = 0.;
					(*npixels)++;
				}
			}
		}
	}
}
