/*  $Id: wrscan7.c,v 1.6 2002/12/17 21:55:15 tom Exp tom $
    $Log: wrscan7.c,v $
 * Revision 1.6  2002/12/17  21:55:15  tom
 * fix pointer/cast errors found with gcc
 *
    Revision 1.5  2000/01/20 21:29:59  tom
    *** empty log message ***

 * Revision 1.4  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.3  1998/12/18  15:21:14  tom
 * omit writing subheader
 *
 * Revision 1.2  1998/12/15  14:52:22  tom
 * debugged version
 *
 * Revision 1.1  1998/12/10  18:42:07  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		wrscan7
 *  Date:           Dec-98
 *  Author:         Tom Videen (from JMO)
 *	Description:	Write scan data for 1 plane of a Version 7 file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/RCS/wrscan7.c,v 1.6 2002/12/17 21:55:15 tom Exp tom $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>
#include <endian.h>

int             wrscan7 (file, indx, matrix)
	ImageFile      *file;
	Index          *indx;
	short          *matrix;
{
	Main_header7   *mh7;
	Scan3D_subheader7 *s3d;
	DataSetType     dtype;
	char           *fname;
	int             xdim, ydim;
	int             begrec, endrec;
	int             matnum, len, i, j;
	int             z_elements, joff, dskptr, group, segment;
	int             slice, stat;
	int             pln1 = 1;
	int             reclen = 1;

	fname = file->name;
	if (file->m7 == NULL) {
		(void) fprintf (stderr, "ERROR [wrscan7]: Version 7 header is not defined for %s\n", fname);
		return (ERROR);
	}
	if (file->m7->mh == NULL) {
		(void) fprintf (stderr, "ERROR [wrscan7]: Version 7 mainheader is not defined for %s\n", fname);
		return (ERROR);
	}
	mh7 = file->m7->mh;
	if (file->m7->s3d == NULL) {
		(void) fprintf (stderr, "ERROR [wrscan7]: Version 7 subheader is not defined for %s\n", fname);
		return (ERROR);
	}
	dtype = (DataSetType) mh7->file_type;
	if (dtype != Short3dSinogram) {
		(void) fprintf (stderr, "ERROR [wrscan7]: Unsupported file type %d in %s\n", dtype, fname);
		return (ERROR);
	}
	s3d = file->m7->s3d;
	xdim = s3d->num_r_elements;
	ydim = s3d->num_angles;
	slice = indx->pln - 1;
	len = xdim * ydim;
	if ((matnum = numcode7 (indx->frm, pln1, indx->gate, indx->data, indx->bed)) == 0) {
		(void) fprintf (stderr, "ERROR [wrscan7]: Could not get matrix number for %s\n", fname);
		return (ERROR);
	}
	if ((begrec = getmatpos (file, matnum, &endrec)) == ERROR) {
		if ((begrec = create_dir7 (file, matnum, reclen)) < 0) {
			(void) fprintf (stderr, "ERROR [wrscan7]: Could not entry for matrix #%x in %s\n",
					matnum, fname);
			return (ERROR);
		}
	}

	/* CONSTCOND  */
	if (__BYTE_ORDER == __LITTLE_ENDIAN)
		swap_byte ((char *) matrix, len * (int) sizeof (short));


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
			(void) fprintf (stderr, "ERROR [wrscan7]: slice > z_elements (%d > %d)\n",
					slice, z_elements);
			return (ERROR);
		}
		dskptr = (begrec + NUMREC_SSHDR - 1) * LENREC + slice * xdim * sizeof (short);

/* Point to correct ring difference group */

		for (i = 0; i < group; i++)
			dskptr += s3d->num_z_elements[i] * len * sizeof (short);

/* Point to correct ring difference sign. */

		if (segment < 0)
			dskptr += z_elements * len * sizeof (short);

/* Write a line for each planar view. */

		for (j = 0, joff = 0; j < ydim; j++, joff += xdim) {
			if ((stat = fseek (file->fp, (long) dskptr, SEEK_SET)) != 0) {
				(void) fprintf (stderr, "ERROR [wrscan7]: during seek; status = %d\n", stat);
				return (ERROR);
			}
			if ((stat = fwrite (&matrix[joff], sizeof (short), xdim, file->fp)) != xdim) {
				(void) fprintf (stderr, "ERROR [wrscan7]: during write; status = %d\n", stat);
				return (ERROR);
			}
			dskptr += xdim * sizeof (short) * s3d->num_z_elements[group];
		}

/*	SINOGRAM MODE */

	} else {
		dskptr = (begrec + NUMREC_SSHDR - 1) * LENREC + slice * len * sizeof (short);
		if ((stat = fseek (file->fp, (long) dskptr, SEEK_SET)) != 0) {
			(void) fprintf (stderr, "ERROR [wrscan7]: during seek; status = %d\n", stat);
			return (ERROR);
		}
		if ((stat = fwrite (matrix, sizeof (short), len, file->fp)) != len) {
			(void) fprintf (stderr, "ERROR [wrscan7]: during write; status = %d\n", stat);
			return (ERROR);
		}
	}
	return (OK);
}
