/*  $Id: rdattn7.c,v 1.5 2003/07/01 19:14:19 tom Exp $
    $Log: rdattn7.c,v $
 * Revision 1.5  2003/07/01  19:14:19  tom
 * *** empty log message ***
 *
 * Revision 1.4  2002/12/20  17:52:47  tom
 * lint
 *
 * Revision 1.3  2002/12/17  21:55:15  tom
 * fix pointer/cast errors found with gcc
 *
    Revision 1.2  2000/09/27 19:03:28  tom
    *** empty log message ***

 * Revision 1.1  2000/01/20  21:29:59  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		rdattn7
 *  Date:           Mar 2003
 *  Author:         Tom Videen
 *	Description:	Get attenuation correction data for 1 plane of a Version 7 file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/rdattn7.c,v 1.5 2003/07/01 19:14:19 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <image.h>
#include <libimage.h>
#include <endian.h>

float          *rdattn7 (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	DataSetType     dtype;
	Attn_subheader7 *atn;
	float          *matrix;
	int             joff, stat;
	int             dskptr, slice, z_elements;
	int             xdim, ydim;
	int             matnum, begrec, endrec;
	long            len;
	int             j;
	int             pln1 = 1;

	dtype = (DataSetType) file->m7->mh->file_type;
	if (dtype != AttenCor) {
		fprintf (stderr, "ERROR [rdattn7]: Unsupported file type %d for %s\n", dtype, file->name);
		return (NULL);
	}
	if (file->fp == NULL) {
		fprintf (stderr, "ERROR [rdattn7]: Could not get file pointer for %s\n", file->name);
		return (NULL);
	}
	if ((get_sa7 (file, indx)) == ERROR) {
		fprintf (stderr, "ERROR [rdattn7]: Could not get subheader for %s\n", file->name);
		return (NULL);
	}
	if ((matnum = numcode7 (indx->frm, pln1, indx->gate, indx->data, indx->bed)) == 0) {
		fprintf (stderr, "ERROR [rdattn7]: Could not get matrix number for %s\n", file->name);
		return (NULL);
	}
	if ((begrec = getmatpos (file, matnum, &endrec)) == ERROR) {
		fprintf (stderr, "ERROR [rdattn7]: Matrix #%x not found\n", matnum);
		return (NULL);
	}
	atn = file->m7->atn;

	xdim = atn->num_r_elements;
	ydim = atn->num_angles;
	len = xdim * ydim;
	slice = indx->pln - 1;
	matrix = (float *) calloc (len, sizeof (float));

/*	VIEW MODE */

	if (atn->storage_order == 0) {
		z_elements = atn->num_z_elements;
		if (slice >= z_elements) {
			fprintf (stderr, "ERROR [rdattn7]: slice > z_elements (%d > %d)\n",
				 slice, z_elements);
			return (NULL);
		}
		dskptr = (begrec + NUMREC_ASHDR - 1) * LENREC + slice * xdim * sizeof (float);

/* Extract a line from each planar view. */

		for (j = 0, joff = 0; j < ydim; j++, joff += xdim) {
			stat = fseek (file->fp, (long) dskptr, SEEK_SET);
			if (stat != 0) {
				fprintf (stderr, "ERROR [rdattn7]: in fseek\n");
				return (NULL);
			}
			stat = fread (&matrix[joff], sizeof (float), xdim, file->fp);
			if (stat != xdim) {
				fprintf (stderr, "ERROR [rdattn7]: reading view\n");
				return (NULL);
			}
			dskptr += xdim * sizeof (float) * z_elements;
		}

/*	SINOGRAM MODE */

	} else {
		dskptr = (begrec + NUMREC_ASHDR - 1) * LENREC + slice * len * sizeof (float);
		stat = fseek (file->fp, (long) dskptr, SEEK_SET);
		if (stat != 0) {
			fprintf (stderr, "ERROR [rdattn7]: in fseek\n");
			return (NULL);
		}
		stat = fread (matrix, sizeof (float), len, file->fp);
		if (stat != len) {
			fprintf (stderr, "ERROR [rdattn7]: reading sinogram\n");
			return (NULL);
		}
	}

	/* CONSTCOND  */
	if (__BYTE_ORDER == __LITTLE_ENDIAN) {
		swap_byte ((char *) matrix, len * (long) sizeof (float));
		swap_word ((short *) matrix, len * (long) sizeof (short));
	}
	return (matrix);
}
