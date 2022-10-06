/*  $Log: wrimgif.c,v $
 * Revision 1.2  2003/07/01  19:14:19  tom
 * *** empty log message ***
 *
 * Revision 1.1  2001/11/21  17:01:28  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		wrimgif
 *  Date:           Mar 2003
 *  Author:         Tom Videen
 *	Description:	Write image data for Interfile image.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/wrimgif.c,v 1.2 2003/07/01 19:14:19 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <endian.h>

#define MAXSCL 16383.0

int             wrimgif (file, image)
	ImageFile      *file;
	float        ***image;
{
	MainHeader_if  *ifh;
	int             fformat;
	char           *fname;
	short          *simg;
	float          *fimg;
	long            len, ix, nbytes;
	int             dim1, dim2, dim3;
	int             i, j, pln;

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
	nbytes = len * sizeof (short);

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
		/* CONSTCOND */
		if ((ifh->big_endian == 1 && __BYTE_ORDER != __BIG_ENDIAN) ||
		    (ifh->big_endian == 0 && __BYTE_ORDER != __LITTLE_ENDIAN))
			swap_byte ((char *) simg, nbytes);
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
		/* CONSTCOND */
		if ((ifh->big_endian == 1 && __BYTE_ORDER != __BIG_ENDIAN) ||
		    (ifh->big_endian == 0 && __BYTE_ORDER != __LITTLE_ENDIAN)) {
			swap_word ((short *) fimg, nbytes / 2);
			swap_byte ((char *) fimg, nbytes);
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
