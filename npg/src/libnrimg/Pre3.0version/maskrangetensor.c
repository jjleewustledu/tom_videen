/*  ================================================================================
 *  File Name:      maskrangetensor.c
 *  Date:           Dec-98
 *  Author:         Tom Videen
 *  Description:    Create an image mask by setting all elements outside the range 
 *					min to max to 0.
 *  =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libnrimg/Pre3.0version/RCS/maskrangetensor.c,v 1.2 1999/12/13 18:21:08 tom Exp $";
#endif

#include <stdio.h>
#include <image.h>

void            maskrangetensor (file, img, min, max, npixels)
	ImageFile      *file;
	float        ***img;			 /* masked image (output) */
	float           min, max;
	long           *npixels;		 /* total pixels within mask */
{
	int             i, j, pln;
	long            dim1, dim2, dim3;

	dim1 = (long) file->sh->dim1;
	dim2 = (long) file->sh->dim2;
	dim3 = (long) file->sh->dim3;
	*npixels = 0;

	for (pln = 1; pln <= dim3; pln++) {
		for (j = 1; j <= dim2; j++) {
			for (i = 1; i <= dim1; i++) {
				if (img[pln][j][i] < min || img[pln][j][i] > max)
					img[pln][j][i] = 0;
				else
					*npixels += 1;
			}
		}
	}
}
