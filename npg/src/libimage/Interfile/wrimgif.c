/*	$Id$
 *	$Log$
 */
/* =============================================================================
 *  Function:       wrimgif
 *  Date:           Nov-95
 *  Author:         John Ollinger
 *  Description:    Write Interfile image.
 *  Description:    Get Interfile header.
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
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <image.h>
#include <libimage.h>

int             get_mhif (file, indx, image)
	ImageFile      *file;
	Index          *indx;
	float          *image;
{
	FILE           *fp;
	MainHeader_if  *mh;
	SubHeader_if   *sh;
	int             i, count, dskptr, stat, maxnum;
	int             frame, plane;
	char           *hdrfile;
	float           scl, min, max;
	short          *shtimg;
	char           *bytimg;

/*	Write one plane at a time */

	plane = indx->pln;
	plane = indx->frm;
	if (plane > 0) {
		dskptr = ((frame - 1) * mh->dim3 + (plane - 1)) * sh->dim1 * sh->dim2;
		count = sh->dim1 * sh->dim2;

/*	Write entire volume */

	} else {
		dskptr = (frame - 1) * mh->dim3 * sh->dim1 * sh->dim2;
		count = sh->dim1 * sh->dim2 * mh->dim3;
	}

	if (mh->number_format == SHORT_IF || mh->number_format == BYTE_IF) {
		if (mh->number_format == BYTE_IF)
			maxnum = 127;
		else
			maxnum = 32767;
		max = -1.e20;
		min = 1.e20;
		for (i = 0; i < count; i++) {
			if (image[i] > max)
				max = image[i];
			if (image[i] < min)
				min = image[i];
		}
		if (max > -min) {
			scl = maxnum / max;
			max = maxnum;
			min = min * scl;
		} else {
			scl = -maxnum / min;
			max = max * scl;
			min = -maxnum;
		}
	}
	sh->storage_sf = 1. / scl;
	hdrfile = header_file (file->name);
	mh->data_file = file->name;
	if ((open_if (file)) == NULL) {
		fprintf (stderr, "ERROR [wrimgif]: Cannot open %s\n", mh->data_file);
		return (ERROR);
	}
	if (plane > 0)
		mh->scale_factor = 0.;		 /* Invalid value if scaled by plane. */
	else
		mh->scale_factor = sh->storage_sf;

	if (plane <= 1 && frame == 1) {
		mh->dim1 = sh->dim1;
		mh->dim2 = sh->dim2;
		mh->voxel_size_1 = sh->voxel_size_1;
		mh->voxel_size_2 = sh->voxel_size_2;
		if ((putmhdr_if (mh, hdrfile)) != NULL) {
			fprintf (stderr, "ERROR [wrimgif]: Cannot write main header to %s\n", mh->data_file);
			return (ERROR);
		}
	}
	if (plane > 0) {
		if ((putsbhdr_if (sh, hdrfile, plane, frame)) != NULL) {
			fprintf (stderr, "ERROR [wrimgif]: Cannot write subheader to %s\n", mh->data_file);
			return (ERROR);
		}
	}
	if (mh->number_format == BYTE_IF) {
		bytimg = (char *) malloc (count * sizeof (char));
		for (i = 0; i < count; i++)
			bytimg[i] = scl * image[i];
		if (fseek (fp, (long) dskptr, SEEK_SET) != NULL)
			return (NULL);
		if ((stat = fwrite (bytimg, sizeof (char), count, fp)) != count) {
			fprintf (stderr, "ERROR [wrimgif]: during write: status = %d\n", stat);
			return (ERROR);
		}
		free (bytimg);
	} else if (mh->number_format == SHORT_IF) {

		shtimg = (short *) malloc (count * sizeof (short));
		for (i = 0; i < count; i++)
			shtimg[i] = scl * image[i];
		dskptr *= 2;
		if (fseek (fp, (long) dskptr, SEEK_SET) != NULL)
			return (NULL);
		if ((stat = fwrite (shtimg, sizeof (short), count, fp)) != count) {
			fprintf (stderr, "ERROR [wrimgif]: during write: status = %d\n", stat);
			return (ERROR);
		}
		free (shtimg);
	} else if (mh->number_format == FLOAT_IF) {
		dskptr *= 4;
		if (fseek (fp, (long) dskptr, SEEK_SET) != NULL)
			return (NULL);
		if ((stat = fwrite (image, sizeof (float), count, fp)) != count) {
			fprintf (stderr, "ERROR [wrimgif]: during write: status = %d\n", stat);
			return (ERROR);
		}
	} else {
		fprintf (stderr, "ERROR [wrimgifh]: Unsupported data type: %d\n", mh->number_format);
		return (ERROR);
	}

	free (hdrfile);
	return (NULL);
}
