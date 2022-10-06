/*	$Id$
 *	$Log$
 */
/* =============================================================================
 *  Function:       rdimgif
 *  Date:           Nov-95
 *  Author:         John Ollinger
 *  Description:    Read Interfile image.
 *  History:
 *      Mar-96  TOV Modified name, return value, and calling parameters.
 *
 *	Copyright 1991-1996, Washington University.
 *	All rights reserved. Unauthorized reproduction prohibited.
 * =============================================================================
 */

#ifndef lint
static char     rcsid[] = "$Header$";

#endif

#include <stdio.h>
#include <unistd.h>
#include <image.h>
#include <libimage.h>

float          *rdimgif (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	MainHeader_if  *mh;
	SubHeader_if   *sh;
	FILE           *fp;
	int             i, bytes_per_pix, count, stat, pln1;
	int             plane;
	long            dskptr;
	char           *bytimg, *hdrfile;
	short          *shtimg;
	float          *fltimg, scale_factor;

	hdrfile = header_file (file->name);
	if ((get_mhif (file)) != OK) {
		fprintf (stderr, "ERROR [rdimgif]: Cannot read mainheader from %s\n", file->name);
		return (NULL);
	}
	plane = indx->pln;
	if (plane <= 0)
		pln1 = 1;
	else
		pln1 = plane;
	if ((get_shif (file, indx)) != OK) {
		fprintf (stderr, "ERROR [rdimgif]: Cannot read subheader from %s\n", file->name);
		return (NULL);
	}
	if (sh->scale_factor > 0)
		scale_factor = sh->scale_factor;
	else {
		if (mh->scale_factor > 0)
			scale_factor = mh->scale_factor;
		else
			scale_factor = 1;
	}

	if ((fp = getfp (mh->data_file, "r")) == NULL) {
		fprintf (stderr, "ERROR [rdimgif]: Cannot open %s\n", mh->data_file);
		return (NULL);
	}
/*	Read one plane at a time */

	if (plane > 0) {
		dskptr = ((frame - 1) * mh->dim3 + (plane - 1)) * sh->dim1 * sh->dim2;
		count = sh->dim1 * sh->dim2;

/*	Read entire volume */
	} else {
		dskptr = (frame - 1) * mh->dim3 * sh->dim1 * sh->dim2;
		count = sh->dim1 * sh->dim2 * mh->dim3;
	}

	if (mh->number_format == BYTE_IF)
		bytes_per_pix = 1;
	else if (mh->number_format == SHORT_IF)
		bytes_per_pix = 2;
	else if (mh->number_format == FLOAT_IF)
		bytes_per_pix = 4;

	dskptr *= bytes_per_pix;
	if ((stat = fseek (fp, (long) dskptr, SEEK_SET)) != NULL)
		return (NULL);

	fltimg = (float *) malloc (count * sizeof (float));
	if (mh->number_format == BYTE_IF) {
		bytimg = (char *) malloc (count * sizeof (char));
		if ((stat = fread (bytimg, sizeof (char), count, fp)) != count)
			return (NULL);
		for (i = 0; i < count; i++)
			fltimg[i] = scale_factor * bytimg[i];
		free (bytimg);
	} else if (mh->number_format == SHORT_IF) {

		shtimg = (short *) malloc (count * sizeof (short));
		if ((stat = fread (shtimg, sizeof (short), count, fp)) != count)
			return (NULL);
		for (i = 0; i < count; i++)
			fltimg[i] = scale_factor * shtimg[i];
		free (shtimg);
	} else if (mh->number_format == FLOAT_IF) {
		if ((stat = fread (fltimg, sizeof (float), count, fp)) != count)
			return (NULL);
		for (i = 0; i < count; i++)
			fltimg[i] *= scale_factor;
	}
	free_ifmh (mh);
	free (sh);
	free (hdrfile);
	return (fltimg);
}
