/*  ================================================================================
 *  File Name:      maskobjtensor.c
 *  Date:           Nov-98
 *  Author:         Tom Videen
 *  Description:    Create an image mask by setting all elements >= min AND <= max
 *		which are connected to [x,y,z] to 1 and all other pixels to 0.
 *		Total pixels in the mask are returned in npixels.
 *	History:
 *		Revision of maskalltensor.c
 *  =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libnrimg/Pre3.0version/RCS/maskobjtensor.c,v 1.3 1999/12/13 18:21:08 tom Exp $";
#endif

#include <stdio.h>
#include <image.h>

#define OUTSIDE		0.
#define INSIDE		1.
#define NEITHER		2.

void            maskobjtensor (file, img, min, max, xc, yc, zc, npixels)
	ImageFile      *file;
	float        ***img;			 /* masked image (output) */
	int             min, max;		 /* save all in this range */
	long            xc, yc, zc;
	long           *npixels;		 /* total pixels within mask */
{
	int             more;
	int             i, j, pln;
	long            dim1, dim2, dim3;

	dim1 = (long) file->sh->dim1;
	dim2 = (long) file->sh->dim2;
	dim3 = (long) file->sh->dim3;
	*npixels = 0;
/*
 *	Assign OUTSIDE to all points < min or > max and all others to NEITHER.
 */
	for (pln = 1; pln <= dim3; pln++) {
		for (j = 1; j <= dim2; j++) {
			for (i = 1; i <= dim1; i++) {
				if (img[pln][j][i] < min)
					img[pln][j][i] = OUTSIDE;
				else if  (img[pln][j][i] > max)
					img[pln][j][i] = OUTSIDE;
				else
					img[pln][j][i] = NEITHER;
			}
		}
	}
	img[zc][yc][xc] = INSIDE;

/*
 *	Now, starting with [zc][yc][xc], expand the area included by
 *	assigning INSIDE to all points directly adjacent to a point whose
 *	value is INSIDE and which are currently zero (neither INSIDE nor OUTSIDE).
 *	The following loop is repeated until it cycles with no more points added.
 */

	more = TRUE;
	while (more) {
		more = FALSE;
		for (pln = 1; pln <= dim3; pln++) {
			for (i = 2; i <= dim1 - 1; i++) {
				for (j = 2; j <= dim2 - 1; j++) {
					if (img[pln][j][i] == INSIDE) {
						if (img[pln][j][i - 1] == NEITHER) {
							img[pln][j][i - 1] = INSIDE;
							more = TRUE;
						}
						if (img[pln][j][i + 1] == NEITHER) {
							img[pln][j][i + 1] = INSIDE;
							more = TRUE;
						}
						if (img[pln][j - 1][i] == NEITHER) {
							img[pln][j - 1][i] = INSIDE;
							more = TRUE;
						}
						if (img[pln][j + 1][i] == NEITHER) {
							img[pln][j + 1][i] = INSIDE;
							more = TRUE;
						}
						if (pln > 1 && pln < dim3) {
							if (img[pln - 1][j][i] == NEITHER) {
								img[pln - 1][j][i] = INSIDE;
								more = TRUE;
							}
							if (img[pln + 1][j][i] == NEITHER) {
								img[pln + 1][j][i] = INSIDE;
								more = TRUE;
							}
						}
					}
				}
			}
		}
	}

/*
 *	All points that are NEITHER are assigned 0
 */
	for (pln = 1; pln <= dim3; pln++) {
		for (j = 1; j <= dim2; j++) {
			for (i = 1; i <= dim1; i++) {
				if (img[pln][j][i] == INSIDE)
					(*npixels)++;
				else
					img[pln][j][i] = 0.;
			}
		}
	}
}
