/*  $Id */
/*  $Log */
/* =============================================================================
 *	Function:		rdimg7_decaycor
 *  Date:           Aug 2008
 *  Author:         Tom Videen
 *	Description:	Get image data from Version 7 file.
 *					Do not divide by decay_cor
 * =============================================================================
 */
/*@unused@*/
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/RCS/rdimg7.c,v 2.5 2004/08/31 14:44:06 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>
#include <libimage.h>
#include <endian.h>

float          *rdimg7_decaycor (file, indx)
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
	int             matnum, begrec, endrec;
	long            nrec = NUMREC_ISHDR;
	long            len;
	int             calibrated;
	int             scatter_corrected;
	int             i;
	int             pln1 = 1;

	dtype = (DataSetType) file->m7->mh->file_type;
	if (dtype != PetVolume) {
		(void) fprintf (stderr, "ERROR [rdimg7_decaycor]: Unsupported file type %d for %s\n", dtype, file->name);
		return (NULL);
	}
	if (file->fp == NULL) {
		(void) fprintf (stderr, "ERROR [rdimg7_decaycor]: Could not get file pointer for %s\n", file->name);
		return (NULL);
	}
	if ((matnum = numcode7 (indx->frm, pln1, indx->gate, indx->data, indx->bed)) == 0) {
		(void) fprintf (stderr, "ERROR [rdimg7_decaycor]: Could not get matrix number for %s\n", file->name);
		return (NULL);
	}
	if ((get_sh7 (file, indx)) == ERROR) {
		(void) fprintf (stderr, "ERROR [rdimg7_decaycor]: Could not get subheader for %s\n", file->name);
		return (NULL);
	}
	mh7 = file->m7->mh;
	sh7 = file->m7->sh;
	xdim = sh7->x_dimension;
	ydim = sh7->y_dimension;
	zdim = sh7->z_dimension;

	if (mh7->calibration_factor == 1)
		calibrated = 0;
	else
		calibrated = 1;

	if ((sh7->filter_scatter_fraction > 0) && (sh7->filter_scatter_slope > 0)
	    || ((sh7->processing_code & 64) > 0) || ((sh7->processing_code & 128) > 0))
		scatter_corrected = 1;
	else
		scatter_corrected = 0;

	if ((begrec = getmatpos (file, matnum, &endrec)) == ERROR) {
		(void) fprintf (stderr, "ERROR [rdimg7_decaycor]: Matrix #%x not found\n", (unsigned) matnum);
		return (NULL);
	}
	if (indx->pln > 0) {
		len = xdim * ydim;
		dptr = (long) (begrec + nrec - 1) * LENREC +
			(long) (indx->pln - 1) * (long) len *(long) sizeof (short);
	} else {
		len = xdim * ydim * zdim;
		dptr = (long) (begrec - 1) * LENREC;
	}
	if ((fseek (file->fp, (long) dptr, SEEK_SET)) != 0)
		return (NULL);

	simg = (short *) calloc (len, sizeof (short));
	if (simg == NULL) {
		(void) fprintf (stderr, "ERROR [rdimg7_decaycor]: calloc simg\n");
		return (NULL);
	}
	if (fread (simg, sizeof (short), len, file->fp) != len) {
		(void) fprintf (stderr, "ERROR [rdimg7_decaycor]: During read\n");
		free (simg);
		return (NULL);
	}
	/* CONSTCOND  */
	if (__BYTE_ORDER == __LITTLE_ENDIAN)
		swap_byte ((char *) simg, len * (long) sizeof (short));

	matrix = (float *) calloc (len, sizeof (float));
	if (matrix == NULL) {
		(void) fprintf (stderr, "ERROR [rdimg7_decaycor]: Could not calloc memory for matrix\n");
		free (simg);
		return (NULL);
	}
	if (calibrated == 0 || scatter_corrected == 0)
		scl = REV7NSCALE;		 /* non-quantitative image; Ver 7 reconstruction */
	else
		scl = REV7SCALER;		 /* quantitative image; Ver 7 reconstruction */

	if (mh7->system_type == 220)
		scaler = mh7->calibration_factor * sh7->scale_factor;
	else if (sh7->frame_duration == 0)
		scaler = ATNSCALER * sh7->scale_factor;
	else if (strncmp (mh7->data_units, "1/cm", 4) == 0)
		scaler = ATNSCALER * sh7->scale_factor;
	else
		scaler = (scl * mh7->calibration_factor * sh7->scale_factor *
			  (float) sh7->frame_duration);

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
