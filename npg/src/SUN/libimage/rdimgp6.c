/*  $Id: rdimgp6.c,v 1.3 1999/12/13 17:45:50 tom Exp $
 *  $Log: rdimgp6.c,v $
 *  Revision 1.3  1999/12/13 17:45:50  tom
 *  tov revision
 *
 * Revision 1.2  1997/12/02  22:46:28  tom
 * modify for dynamic
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		rdimgp6
 *  Date:           Nov-95
 *  Author:         Tom Videen
 *	Description:	Get image data from PETT VI file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/rdimgp6.c,v 1.3 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <image.h>
#include <libimage.h>

float          *rdimgp6 (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	short          *simg;
	float          *img;
	long            offset, slices, n;
	long            i, len;

	if ((get_hp6 (file, indx)) == ERROR) {
		(void) fprintf (stderr, "ERROR [rdimgp6]: Could not read header of %s plane %d\n",
			 file->name, indx->pln);
		return (NULL);
	}
	len = PETT6_DIM * PETT6_DIM;
	simg = (short *) malloc (len * sizeof (short));

    slices = file->p6->number_of_slices;
    if (slices == 0)
        slices = 7;
    n = (long) ((indx->frm) - 1) * slices;
    offset = (n + (long) indx->pln - 1) * PETT6_SLICE_BYTES + RECLEN;
	if ((fseek (file->fp, offset, SEEK_SET)) != 0) {
		(void) fprintf (stderr, "ERROR [rdimgp6]: Reading %s plane %d\n", file->name, indx->pln);
		return (NULL);
	}
	if ((fread (simg, sizeof (short), len, file->fp)) != len) {
		(void) fprintf (stderr, "ERROR [rdimgp6]: Reading %s plane %d\n", file->name, indx->pln);
		return (NULL);
	}
	if ((img = (float *) malloc (len * sizeof (float))) == NULL) {
		(void) fprintf (stderr, "ERROR [rdimgp6]: Could not malloc memory for img\n");
		free (simg);
		return (NULL);
	}
	for (i = 0; i < len; i++)
		img[i] = (float) simg[i];
	free (simg);
	return (img);
}
