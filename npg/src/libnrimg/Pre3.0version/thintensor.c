/* =============================================================================
 *	Function:		thintensor
 *  Date:           Dec-95
 *  Author:         Tom Videen
 *	Description:	Set outer pixels to zero for each plane of an image.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libnrimg/Pre3.0version/RCS/thintensor.c,v 1.3 1996/08/05 14:16:08 tom Exp $";
#endif

#include <stdio.h>
#include <image.h>

void            thintensor (file, img, count)
	ImageFile      *file;
	float        ***img;		 /* masked image (output) */
	int             count;		 /* number of pixels to cut */

{
	int             n;
	long            dim1, dim2, dim3;
	long            i, j, k;

	if (count <= 0)
		return;
	dim1 = (long) file->sh->dim1;
	dim2 = (long) file->sh->dim2;
	dim3 = (long) file->sh->dim3;

	for (k = 1; k <= dim3; k++) {
		for (j = 1; j <= dim2; j++) {
			n = count;
			for (i = 1; i <= dim1; i++) {
				if (img[k][j][i] > 0) {
					img[k][j][i] = 0;
					n--;
				}
				if (n == 0)
					break;
			}
		}
		for (j = 1; j <= dim2; j++) {
			n = count;
			for (i = dim1; i >= 1; i--) {
				if (img[k][j][i] > 0) {
					img[k][j][i] = 0;
					n--;
				}
				if (n == 0)
					break;
			}
		}
		for (i = 1; i <= dim1; i++) {
			n = count;
			for (j = 1; j <= dim2; j++) {
				if (img[k][j][i] > 0) {
					img[k][j][i] = 0;
					n--;
				}
				if (n == 0)
					break;
			}
		}
		for (i = 1; i <= dim1; i++) {
			n = count;
			for (j = dim2; j >= 1; j--) {
				if (img[k][j][i] > 0) {
					img[k][j][i] = 0;
					n--;
				}
				if (n == 0)
					break;
			}
		}
	}
}
