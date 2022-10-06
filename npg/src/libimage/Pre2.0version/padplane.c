/*  $Id: padplane.c,v 1.2 1999/12/13 17:45:50 tom Exp $
    $Log: padplane.c,v $
 * Revision 1.2  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		padplane
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Pad a 2-D image (single plane) with zeros in x- and y-dimensions.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/padplane.c,v 1.2 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

float          *padplane (file, img, newx, newy)
	ImageFile      *file;
	float          *img;
	int             newx;
	int             newy;

{
	float          *pimg;
	int             xdim, ydim, len;
	int             ix, jx, i, j, m, n;

	xdim = file->sh->dim1;
	ydim = file->sh->dim2;
	if (xdim >= newx && ydim >= newy)
		return (img);

	len = newx * newy;
	if ((pimg = (float *) calloc (len, sizeof (float))) == NULL) {
		(void) fprintf (stderr, "ERROR [padxyimg]: Could not malloc memory for pimg\n");
		return (NULL);
	}
	if (newx >= xdim)
		ix = (newx - xdim) / 2;
	else
		ix = 0;
	if (newy >= ydim)
		jx = (newy - ydim) / 2;
	else
		jx = 0;

	for (j = 0; j < ydim; j++) {
		n = ((j + jx) * newx) + ix;
		m = (j * xdim);
		for (i = 0; i < xdim; i++)
			pimg[n + i] = img[m + i];
	}
	file->sh->dim1 = newx;
	file->sh->dim2 = newy;
	free (img);
	return (pimg);
}
