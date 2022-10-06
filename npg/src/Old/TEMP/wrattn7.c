/*  $Id: wrattn7.c,v 1.3 2002/12/17 21:55:15 tom Exp tom $
    $Log: wrattn7.c,v $
 * Revision 1.3  2002/12/17  21:55:15  tom
 * fix pointer/cast errors found with gcc
 *
    Revision 1.2  2000/09/28 14:44:29  tom
    fix to match ECAT7 attn file

 * Revision 1.1  2000/01/20  21:29:59  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		wrattn7
 *  Date:           Mar 2003
 *  Author:         Tom Videen
 *	Description:	Write scan data for 1 plane of a Version 7 attenuation file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/RCS/wrattn7.c,v 1.3 2002/12/17 21:55:15 tom Exp tom $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>
#include <endian.h>

int             wrattn7 (file, indx, matrix)
	ImageFile      *file;
	Index          *indx;
	float          *matrix;
{
	Main_header7   *mh7;
	Attn_subheader7 *atn;
	DataSetType     dtype;
	char           *fname;
	int            *zero;
	int             xdim, ydim;
	int             begrec, endrec;
	int             matnum, len, j;
	int             z_elements, joff, dskptr;
	int             slice, stat;
	int             pln1 = 1;
	int             reclen = 1;

	fname = file->name;
	if (file->m7 == NULL) {
		(void) fprintf (stderr, "ERROR [wrattn7]: Version 7 header is not defined for %s\n", fname);
		return (ERROR);
	}
	if (file->m7->mh == NULL) {
		(void) fprintf (stderr, "ERROR [wrattn7]: Version 7 mainheader is not defined for %s\n", fname);
		return (ERROR);
	}
	mh7 = file->m7->mh;
	if (file->m7->atn == NULL) {
		(void) fprintf (stderr, "ERROR [wrattn7]: Version 7 subheader is not defined for %s\n", fname);
		return (ERROR);
	}
	dtype = (DataSetType) mh7->file_type;
	if (dtype != AttenCor) {
		(void) fprintf (stderr, "ERROR [wrattn7]: Unsupported file type %d in %s\n", dtype, fname);
		return (ERROR);
	}
	atn = file->m7->atn;
	xdim = atn->num_r_elements;
	ydim = atn->num_angles;
	len = xdim * ydim;
	slice = indx->pln - 1;

	if ((matnum = numcode7 (indx->frm, pln1, indx->gate, indx->data, indx->bed)) == 0) {
		(void) fprintf (stderr, "ERROR [wrattn7]: Could not get matrix number for %s\n", fname);
		return (ERROR);
	}
	if ((begrec = getmatpos (file, matnum, &endrec)) == ERROR) {
		if ((begrec = create_dir7 (file, matnum, reclen)) < 0) {
			(void) fprintf (stderr, "ERROR [wrattn7]: Could not entry for matrix #%x in %s\n",
					matnum, fname);
			return (ERROR);
		}
	}
	if ((zero = (int *) calloc (54, sizeof (int))) == NULL) {
		fprintf (stderr, "ERROR [wrattn7]: Cannot calloc zero\n");
		return (ERROR);
	}
	dskptr = 1300;
	if ((stat = fseek (file->fp, (long) dskptr, SEEK_SET)) != 0) {
		(void) fprintf (stderr, "ERROR [wrattn7]: during seek; status = %d\n", stat);
		return (ERROR);
	}
	if ((stat = fwrite (zero, sizeof (int), 54, file->fp)) != 54) {
		(void) fprintf (stderr, "ERROR [wrattn7]: during write; status = %d\n", stat);
		return (ERROR);
	}
	free (zero);

	/* CONSTCOND  */
	if (__BYTE_ORDER == __LITTLE_ENDIAN) {
		swap_word ((short *) matrix, len * (int) sizeof (short));
		swap_byte ((char *) matrix, len * (int) sizeof (float));
	}

/*	VIEW MODE */

	if (atn->storage_order == 0) {
		z_elements = atn->num_z_elements;
		if (slice >= z_elements) {
			(void) fprintf (stderr, "ERROR [wrattn7]: slice > z_elements (%d > %d)\n",
					slice, z_elements);
			return (ERROR);
		}
		dskptr = (begrec + NUMREC_ASHDR - 1) * LENREC + slice * xdim * sizeof (float);

/* Write a line for each planar view. */

		for (j = 0, joff = 0; j < ydim; j++, joff += xdim) {
			if ((stat = fseek (file->fp, (long) dskptr, SEEK_SET)) != 0) {
				(void) fprintf (stderr, "ERROR [wrattn7]: during seek; status = %d\n", stat);
				return (ERROR);
			}
			if ((stat = fwrite (&matrix[joff], sizeof (float), xdim, file->fp)) != xdim) {
				(void) fprintf (stderr, "ERROR [wrattn7]: during write; status = %d\n", stat);
				return (ERROR);
			}
			dskptr += xdim * sizeof (float) * atn->num_z_elements;
		}

/*	SINOGRAM MODE */

	} else {
		dskptr = (begrec + NUMREC_ASHDR - 1) * LENREC + slice * len * sizeof (float);
		if ((stat = fseek (file->fp, (long) dskptr, SEEK_SET)) != 0) {
			(void) fprintf (stderr, "ERROR [wrattn7]: during seek; status = %d\n", stat);
			return (ERROR);
		}
		if ((stat = fwrite (matrix, sizeof (float), len, file->fp)) != len) {
			(void) fprintf (stderr, "ERROR [wrattn7]: during write; status = %d\n", stat);
			return (ERROR);
		}
	}
	return (OK);
}
