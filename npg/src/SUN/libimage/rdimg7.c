/*  $Id: rdimg7.c,v 1.10 2002/12/17 21:55:15 tom Exp $
    $Log: rdimg7.c,v $
    Revision 1.10  2002/12/17 21:55:15  tom
    fix pointer/cast errors found with gcc

    Revision 1.9  2002/05/30 13:58:03  tom
    use REVNSCALE for non-calibrated images

 * Revision 1.8  2000/09/27  19:03:28  tom
 * *** empty log message ***
 *
 * Revision 1.7  2000/07/12  19:50:51  tom
 * *** empty log message ***
 *
 * Revision 1.6  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.5  1999/04/07  16:30:12  tom
 * Initial revision
 *
 * Revision 1.4  1997/11/24  17:56:45  tom
 * update sh start_time and duration
 *
 * Revision 1.3  1996/08/05  14:13:27  tom
 * move scaling from readtensor & rdtensor to this function
 *
 * Revision 1.2  1996/05/24  17:18:23  tom
 * divide pixels by decay_corr_fctr and set this to 1
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		rdimg7
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Get image data from Version 7 file.
 * =============================================================================
 */

#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/rdimg7.c,v 1.10 2002/12/17 21:55:15 tom Exp $";

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>
#include <libimage.h>

float          *rdimg7 (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	DataSetType     dtype;
	Main_header7   *mh7;
	Image_subheader7 *sh7;
	float          *matrix, scaler, scl;
	short          *simg;
	long            dptr;
	int             xdim, ydim, zdim;
	int             matnum, begrec, endrec, nrec_shdr, len;
	int             pln1 = 1;
	int             i;

	dtype = (DataSetType) file->m7->mh->file_type;
	if (dtype != PetVolume) {
		(void) fprintf (stderr, "ERROR [rdimg7]: Unsupported file type %d for %s\n", dtype, file->name);
		return (NULL);
	}
	if (file->fp == NULL) {
		(void) fprintf (stderr, "ERROR [rdimg7]: Could not get file pointer for %s\n", file->name);
		return (NULL);
	}
	if ((matnum = numcode7 (indx->frm, pln1, indx->gate, indx->data, indx->bed)) == 0) {
		(void) fprintf (stderr, "ERROR [rdimg7]: Could not get matrix number for %s\n", file->name);
		return (NULL);
	}
	if ((get_sh7 (file, indx)) == ERROR) {
		(void) fprintf (stderr, "ERROR [rdimg7]: Could not get subheader for %s\n", file->name);
		return (NULL);
	}
	mh7 = file->m7->mh;
	sh7 = file->m7->sh;
	xdim = sh7->x_dimension;
	ydim = sh7->y_dimension;
	zdim = sh7->z_dimension;
	nrec_shdr = NUMREC_ISHDR;

	if ((begrec = getmatpos (file, matnum, &endrec)) == ERROR) {
		(void) fprintf (stderr, "ERROR [rdimg7]: Matrix #%x not found\n", matnum);
		return (NULL);
	}
	if (indx->pln > 0) {
		len = xdim * ydim;
		dptr = (long) (begrec + nrec_shdr - 1) * LENREC +
			(long) (indx->pln - 1) * (long) len *(long) sizeof (short);
	} else {
		len = xdim * ydim * zdim;
		dptr = (long) (begrec - 1) * LENREC;
	}
	if ((fseek (file->fp, (long) dptr, SEEK_SET)) != 0)
		return (NULL);

	simg = (short *) calloc (len, sizeof (short));
	if (fread (simg, sizeof (short), len, file->fp) != len) {
		(void) fprintf (stderr, "ERROR [rdimg7]: During read\n");
		return (NULL);
	}
	matrix = (float *) calloc (len, sizeof (float));
	if (matrix == NULL) {
		(void) fprintf (stderr, "ERROR [rdimg7]: Could not calloc memory for matrix\n");
		return (NULL);
	}
	if (mh7->calibration_factor == 1 || sh7->filter_scatter_fraction == 0
	    || sh7->filter_scatter_slope == 0)
		scl = REV7NSCALE;		 /* non-quantitative image and 7.0 release file */
	else
		scl = REV7SCALER;		 /* quantitative image */

	if (sh7->frame_duration == 0)
		scaler = ATNSCALER * sh7->scale_factor;
	else if (strncmp (mh7->data_units, "1/cm", 4) == 0)
		scaler = ATNSCALER * sh7->scale_factor;
	else
		scaler = (scl * mh7->calibration_factor * sh7->scale_factor *
			  (float) sh7->frame_duration) / sh7->decay_corr_fctr;

	for (i = 0; i < len; i++)
		matrix[i] = scaler * simg[i];

/*	Set decay_corr_fctr to 1 after restoring input pixels */

	sh7->decay_corr_fctr = 1.0;
	file->sh->decay_corr = 1.0;
	file->sh->start_time = sh7->frame_start_time;
	file->sh->duration = sh7->frame_duration;
	free (simg);
	return (matrix);
}
