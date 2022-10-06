/*  $Id: rdscan7.c,v 1.8 2003/07/01 19:14:19 tom Exp $
    $Log: rdscan7.c,v $
 * Revision 1.8  2003/07/01  19:14:19  tom
 * *** empty log message ***
 *
 * Revision 1.7  2002/12/17  21:55:15  tom
 * fix pointer/cast errors found with gcc
 *
    Revision 1.6  2000/09/27 19:03:28  tom
    *** empty log message ***

 * Revision 1.5  2000/01/20  21:29:59  tom
 * *** empty log message ***
 *
 * Revision 1.4  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.3  1998/12/18  15:22:01  tom
 * cosmetic differences
 *
 * Revision 1.2  1998/12/15  14:52:22  tom
 * debugged version
 *
 * Revision 1.1  1998/12/10  18:41:56  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		rdscan7
 *  Date:           Dec-98
 *  Author:         Tom Videen (from JMO)
 *	Description:	Get scan data for 1 plane of a Version 7 file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/rdscan7.c,v 1.8 2003/07/01 19:14:19 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <image.h>
#include <libimage.h>
#include <endian.h>

short          *rdscan7 (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	DataSetType     dtype;
	Scan3D_subheader7 *s3d;
	short          *matrix;
	int             joff, stat;
	int             dskptr, slice, group, segment, z_elements;
	int             xdim, ydim;
	int             matnum, begrec, endrec;
	long            len;
	int             i, j;
	int             pln1 = 1;

	dtype = (DataSetType) file->m7->mh->file_type;
	if (dtype != Short3dSinogram) {
		(void) fprintf (stderr, "ERROR [rdscan7]: Unsupported file type %d for %s\n", dtype, file->name);
		return (NULL);
	}
	if (file->fp == NULL) {
		(void) fprintf (stderr, "ERROR [rdscan7]: Could not get file pointer for %s\n", file->name);
		return (NULL);
	}
	if ((get_ss7 (file, indx)) == ERROR) {
		(void) fprintf (stderr, "ERROR [rdscan7]: Could not get subheader for %s\n", file->name);
		return (NULL);
	}
	if ((matnum = numcode7 (indx->frm, pln1, indx->gate, indx->data, indx->bed)) == 0) {
		(void) fprintf (stderr, "ERROR [rdscan7]: Could not get matrix number for %s\n", file->name);
		return (NULL);
	}
	if ((begrec = getmatpos (file, matnum, &endrec)) == ERROR) {
		(void) fprintf (stderr, "ERROR [rdscan7]: Matrix #%x not found\n", matnum);
		return (NULL);
	}
	s3d = file->m7->s3d;

	xdim = s3d->num_r_elements;
	ydim = s3d->num_angles;
	len = xdim * ydim;
	slice = indx->pln - 1;
	matrix = (short *) calloc (len, sizeof (short));

/*	VIEW MODE */

	if (s3d->storage_order == 0) {
		for (group = 0; s3d->num_z_elements[group] > 0; group++) {
			if ((slice - s3d->num_z_elements[group]) >= 0)
				slice -= s3d->num_z_elements[group];
			else
				break;
		}
		segment = group;
		z_elements = s3d->num_z_elements[group];
		if (group > 0 && slice >= (z_elements / 2)) {
			segment = -segment;
			slice -= z_elements / 2;
		}
		if (group > 0)
			z_elements /= 2;
		if (slice >= z_elements) {
			(void) fprintf (stderr, "ERROR [rdscan7]: slice > z_elements (%d > %d)\n",
					slice, z_elements);
			return (NULL);
		}
		dskptr = (begrec + NUMREC_SSHDR - 1) * LENREC + slice * xdim * sizeof (short);

/* Point to correct ring difference group */

		for (i = 0; i < group; i++)
			dskptr += s3d->num_z_elements[i] * len * sizeof (short);

/* Point to correct ring difference sign */

		if (segment < 0)
			dskptr += z_elements * len * sizeof (short);

/* Extract a line from each planar view. */

		for (j = 0, joff = 0; j < ydim; j++, joff += xdim) {
			stat = fseek (file->fp, (long) dskptr, SEEK_SET);
			if (stat != 0) {
				(void) fprintf (stderr, "ERROR [rdscan7]: in fseek; status=%d\n", stat);
				return (NULL);
			}
			stat = fread (&matrix[joff], sizeof (short), xdim, file->fp);
			if (stat != xdim) {
				(void) fprintf (stderr, "ERROR [rdscan7]: reading; status=%d\n", stat);
				return (NULL);
			}
			dskptr += xdim * sizeof (short) * z_elements;
		}

/*	SINOGRAM MODE */

	} else {
		dskptr = (begrec + NUMREC_SSHDR - 1) * LENREC + slice * len * sizeof (short);
		stat = fseek (file->fp, (long) dskptr, SEEK_SET);
		if (stat != 0) {
			(void) fprintf (stderr, "ERROR [rdscan7]: in fseek; status=%d\n", stat);
			return (NULL);
		}
		stat = fread (matrix, sizeof (short), len, file->fp);
		if (stat != len) {
			(void) fprintf (stderr, "ERROR [rdscan7]: reading; status=%d\n", stat);
			return (NULL);
		}
	}

	/* CONSTCOND  */
	if (__BYTE_ORDER == __LITTLE_ENDIAN)
		swap_byte ((char *) matrix, len * (long) sizeof (short));

	return (matrix);
}
