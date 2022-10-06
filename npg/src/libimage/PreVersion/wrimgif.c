/*  $Log: wrimgif.c,v $
 * Revision 1.1  2001/11/21  17:01:28  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		wrimgif
 *  Date:           Nov 2001
 *  Author:         Tom Videen
 *	Description:	Write image data for Interfile image.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/wrimgif.c,v 1.1 2001/11/21 17:01:28 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

#define MAXSCL 16383.0

int             wrimgif (file, image)
	ImageFile      *file;
	float          ***image;
{
	MainHeader_if  *ifh;
	int				fformat;
	char           *fname;
	short          *simg;
	float          *fimg;
	int             dim1, dim2, dim3;
	int             len, i, j, pln, ix;

	fname = file->name;
	fformat = file->mh->file_format;
	if (fformat != INTERFILE) {
		(void) fprintf (stderr, "ERROR [wrimgif]: Not Interfile format: %s\n", fname);
		return (ERROR);
	}
	if (file->ih == NULL) {
		(void) fprintf (stderr, "ERROR [wrimgif]: Interfile header is not defined for %s\n", fname);
		return (ERROR);
	}
	if (file->ih->mh == NULL) {
		(void) fprintf (stderr, "ERROR [wrimgif]: Interfile mainheader is not defined for %s\n", fname);
		return (ERROR);
	}
	ifh = file->ih->mh;

	dim1 = (long) file->sh->dim1;
	dim2 = (long) file->sh->dim2;
	dim3 = (long) file->sh->dim3;
	len = dim1 * dim2 * dim3;

	if (ifh->number_format == SHORT_DATA || ifh->number_format == INT_DATA) {
    	if ((simg = (short *) calloc (len, sizeof (short))) == NULL) {
			fprintf (stderr, "ERROR [wrimgif]: calloc simg for writing %s\n", fname);
			return (ERROR);
		}
		ix = 0;
		for (pln = 1; pln <= dim3; pln++) {
			for (j = 1; j <= dim2; j++) {
				for (i = 1; i <= dim1; i++) {
					simg[ix] = image[pln][j][i];
					ix++;
				}
			}
		}
		if ((fwrite (simg, sizeof (short), len, file->fp)) != len) {
			(void) fprintf (stderr, "ERROR [wrimgif]: writing data to %s\n", fname);
			return (ERROR);
		}
		free (simg);
		return (OK);

	} else if (ifh->number_format == FLOAT_DATA) {
    	if ((fimg = (float *) calloc (len, sizeof (float))) == NULL) {
			fprintf (stderr, "ERROR [wrimgif]: calloc fimg for writing %s\n", fname);
			return (ERROR);
		}
		ix = 0;
		for (pln = 1; pln <= dim3; pln++) {
			for (j = 1; j <= dim2; j++) {
				for (i = 1; i <= dim1; i++) {
					fimg[ix] = image[pln][j][i];
					ix++;
				}
			}
		}
		if ((fwrite (fimg, sizeof (float), len, file->fp)) != len) {
			(void) fprintf (stderr, "ERROR [wrimgif]: Writing data to %s\n", fname);
			return (ERROR);
		}
		free (fimg);
		return (OK);

	} else {
		(void) fprintf (stderr, "ERROR [wrimgif]: Unsupported file format %d in %s\n", ifh->number_format, fname);
		return (ERROR);
	}
}
