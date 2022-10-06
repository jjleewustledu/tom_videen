/*  $Id: wrimg6.c,v 1.8 2002/12/17 21:55:15 tom Exp $
    $Log: wrimg6.c,v $
 * Revision 1.8  2002/12/17  21:55:15  tom
 * fix pointer/cast errors found with gcc
 *
    Revision 1.7  2000/09/28 14:44:29  tom
    calloc

 * Revision 1.6  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.5  1996/10/30  23:31:08  tom
 * make sure sh6->quant_scale is not Inf
 *
 * Revision 1.4  1996/08/26  17:47:34  tom
 * fix scaling
 *
 * Revision 1.3  1996/08/22  17:28:00  tom
 * correct scaling min/max
 *
 * Revision 1.2  1996/08/05  14:13:34  tom
 * move scaling from writetensor & wrtensor to this functoin
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		wrimg6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Write image data for Version 6 file.
 * =============================================================================
 */

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/wrimg6.c,v 1.8 2002/12/17 21:55:15 tom Exp $";

#endif

#include <stdio.h>
#include <stdlib.h>
#include <values.h>
#include <image.h>
#include <matrix.h>

#define MAXSCL 32767.0

int             wrimg6 (file, indx, img)
	ImageFile      *file;
	Index          *indx;
	float          *img;
{
	Main_header    *mh6;
	Image_subheader *sh6;
	MatrixData     *mdata;
	MatrixFile     *mfp;
	DataSetType     dtype;
	char           *fname;
	int             matnum;
	int             data_type = 0;
	int             xdim, ydim;
	int             i;
	float           scaler, max, min;
	short          *simg;

	fname = file->name;
	if (file->m6 == NULL) {
		(void) fprintf (stderr, "ERROR [wrimg6]: Version 6 header is not defined for %s\n", fname);
		return (ERROR);
	}
	if (file->m6->mh == NULL) {
		(void) fprintf (stderr, "ERROR [wrimg6]: Version 6 mainheader is not defined for %s\n", fname);
		return (ERROR);
	}
	mh6 = file->m6->mh;
	if (file->m6->sh == NULL) {
		(void) fprintf (stderr, "ERROR [wrimg6]: Version 6 subheader is not defined for %s\n", fname);
		return (ERROR);
	}
	sh6 = file->m6->sh;
	if (file->m6->mfp == NULL) {
		(void) fprintf (stderr, "ERROR [wrimg6]: File %s has not been opened\n", fname);
		return (ERROR);
	}
	mfp = file->m6->mfp;
	dtype = (DataSetType) mh6->file_type;
	if (dtype != PetImage && dtype != PetVolume) {
		(void) fprintf (stderr, "ERROR [wrimg6]: Unsupported file type %d in %s\n", dtype, fname);
		return (ERROR);
	}
	dtype = PetImage;

	if ((matnum = mat_numcod (indx->frm, indx->pln, indx->gate, indx->data, indx->bed)) == 0) {
		(void) fprintf (stderr, "ERROR [wrimg6]: Could not get matrix number for %s\n", fname);
		return (ERROR);
	}
	mdata = (MatrixData *) calloc (1, sizeof (MatrixData));
	if (mdata == NULL) {
		(void) fprintf (stderr, "ERROR [wrimg6]: Could not calloc memory for mdata\n");
		return (ERROR);
	}
	if (sh6->frame_duration == 0)
		scaler = 1. / ATNSCALER;
	else
		scaler = 1. / ((float) sh6->frame_duration);
	xdim = sh6->dimension_1;
	ydim = sh6->dimension_2;
	min = MAXFLOAT;
	max = -MAXFLOAT;
	for (i = 0; i < xdim * ydim; i++) {
		img[i] *= scaler;
		if (img[i] > max)
			max = img[i];
		if (img[i] < min)
			min = img[i];
	}

	if (sh6->data_type != IEEE_R4) {
		data_type = SUN_I2;
		if (max > -min) {
			scaler = MAXSCL / max;
			max = MAXSCL;
			min = min * scaler;
		} else if (min != 0) {
			scaler = -MAXSCL / min;
			max = max * scaler;
			min = -MAXSCL;
		} else {
			scaler = 0;
			max = 0;
			min = 0;
		}
		simg = (short *) calloc (xdim * ydim, sizeof (short));
		if (simg == NULL) {
			(void) fprintf (stderr, "ERROR [wrimg6]: Could not calloc memory for simg\n");
			return (ERROR);
		}
		for (i = 0; i < xdim * ydim; i++)
			simg[i] = (short) (scaler * img[i]);
		sh6->image_min = (short) min;
		sh6->image_max = (short) max;
		if (scaler * sh6->ecat_calibration_fctr == 0)
			sh6->quant_scale = 1.0;
		else
			sh6->quant_scale = 1. / (scaler * sh6->ecat_calibration_fctr);
		mdata->nblks = xdim * ydim * sizeof (short) / 512;
		mdata->data_ptr = (char *) simg;

	} else {				 /* data_type == IEEE_R4 */
		if (min < 32768 && min > -32768)
			sh6->image_min = (short) min;
		else
			sh6->image_min = 0.;
		if (max < 32768 && max > -32768)
			sh6->image_max = (short) max;
		else
			sh6->image_max = MAXSCL;
		sh6->quant_scale = 1.;
		sh6->ecat_calibration_fctr = 1.;
		mdata->nblks = xdim * ydim * sizeof (float) / 512;
		mdata->data_ptr = (char *) img;
	}

	mdata->mat_type = (int) dtype;
	mdata->shptr = (char *) sh6;
	mdata->matnum = matnum;
	mdata->nviews = ydim;
	mdata->nelements = xdim * ydim;
	mdata->data_type = data_type;
	mfp->mhptr->file_type = (int) dtype;
	mfp->mhptr->sw_version = mh6->sw_version;

	if (matrix_write (mfp, matnum, mdata) == ERROR) {
		(void) fprintf (stderr, "ERROR [wrimg6]: Could not write mdata for %s\n", fname);
		return (ERROR);
	}
	if (data_type != IEEE_R4)
		free (simg);
	free (mdata);
	return (OK);
}
