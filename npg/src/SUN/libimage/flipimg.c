/*  $Id: flipimg.c,v 1.4 2000/09/28 14:44:29 tom Exp $
    $Log: flipimg.c,v $
    Revision 1.4  2000/09/28 14:44:29  tom
    calloc

 * Revision 1.3  2000/01/24  17:32:52  tom
 * flip sign instead of zeroing
 *
 * Revision 1.2  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		flipimg
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Reverse right/left in an image.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/flipimg.c,v 1.4 2000/09/28 14:44:29 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

float          *flipimg (file, img)
	ImageFile      *file;
	float          *img;
{
	float          *rimg;
	int             xdim, ydim, len;
	int             i, j, m, n;

	xdim = file->sh->dim1;
	ydim = file->sh->dim2;
	len = xdim * ydim;
	if ((rimg = (float *) calloc (len, sizeof (float))) == NULL) {
		(void) fprintf (stderr, "ERROR [flipimg]: Could not calloc memory for rimg\n");
		return (NULL);
	}
	for (j = 0; j < ydim; j++) {
		m = j * xdim;
		n = (j + 1) * xdim - 1;
		for (i = 0; i < xdim; i++)
			rimg[m + i] = img[n - i];
	}
	if (file->mh->orientation != 0)
		file->mh->orientation = 0;
	else
		file->mh->orientation = 1;
	free (img);
	return (rimg);
}
