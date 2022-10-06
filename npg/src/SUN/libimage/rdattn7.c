/*  $Id: rdattn7.c,v 1.3 2002/12/17 21:55:15 tom Exp $
    $Log: rdattn7.c,v $
    Revision 1.3  2002/12/17 21:55:15  tom
    fix pointer/cast errors found with gcc

    Revision 1.2  2000/09/27 19:03:28  tom
    *** empty log message ***

 * Revision 1.1  2000/01/20  21:29:59  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		rdattn7
 *  Date:           Jan-00
 *  Author:         Tom Videen
 *	Description:	Get attenuation correction data for 1 plane of a Version 7 file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/rdattn7.c,v 1.3 2002/12/17 21:55:15 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <image.h>
#include <libimage.h>

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
	int             matnum, begrec, endrec, len;
	int             j, pln1 = 1;

	dtype = (DataSetType) file->m7->mh->file_type;
	if (dtype != AttenCor) {
		(void) fprintf (stderr, "ERROR [rdattn7]: Unsupported file type %d for %s\n", dtype, file->name);
		return (NULL);
	}
	if (file->fp == NULL) {
		(void) fprintf (stderr, "ERROR [rdattn7]: Could not get file pointer for %s\n", file->name);
		return (NULL);
	}
	if ((get_sa7 (file, indx)) == ERROR) {
		(void) fprintf (stderr, "ERROR [rdattn7]: Could not get subheader for %s\n", file->name);
		return (NULL);
	}
	if ((matnum = numcode7 (indx->frm, pln1, indx->gate, indx->data, indx->bed)) == 0) {
		(void) fprintf (stderr, "ERROR [rdattn7]: Could not get matrix number for %s\n", file->name);
		return (NULL);
	}
	if ((begrec = getmatpos (file, matnum, &endrec)) == ERROR) {
		(void) fprintf (stderr, "ERROR [rdattn7]: Matrix #%x not found\n", matnum);
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
			(void) fprintf (stderr, "ERROR [rdattn7]: slice > z_elements (%d > %d)\n",
				slice, z_elements);
			return (NULL);
		}
		dskptr = (begrec + NUMREC_ASHDR - 1) * LENREC + slice * xdim * sizeof (float);

/* Extract a line from each planar view. */

		for (j = 0, joff = 0; j < ydim; j++, joff += xdim) {
			if (fseek (file->fp, (long) dskptr, SEEK_SET) != 0) {
				(void) fprintf (stderr, "ERROR [rdattn7]: in fseek; status=%d\n", stat);
				return (NULL);
			}
			if ((stat = fread (&matrix[joff], sizeof (float), xdim, file->fp)) != xdim) {
				(void) fprintf (stderr, "ERROR [rdattn7]: reading; status=%d\n", stat);
				return (NULL);
			}
			dskptr += xdim * sizeof (float) * z_elements;
		}

/*	SINOGRAM MODE */

	} else {
		dskptr = (begrec + NUMREC_ASHDR - 1) * LENREC + slice * len * sizeof (float);
		if (fseek (file->fp, (long) dskptr, SEEK_SET) != 0) {
			(void) fprintf (stderr, "ERROR [rdattn7]: in fseek; status=%d\n", stat);
			return (NULL);
		}
		if ((stat = fread (matrix, sizeof (float), len, file->fp)) != len) {
			(void) fprintf (stderr, "ERROR [rdattn7]: reading; status=%d\n", stat);
			return (NULL);
		}
	}

	return (matrix);
}
