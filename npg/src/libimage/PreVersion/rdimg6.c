/*  $Id: rdimg6.c,v 1.8 2002/12/17 21:55:15 tom Exp $
    $Log: rdimg6.c,v $
 * Revision 1.8  2002/12/17  21:55:15  tom
 * fix pointer/cast errors found with gcc
 *
    Revision 1.7  1999/12/13 17:45:50  tom
    tov revision

 * Revision 1.6  1997/11/24  17:57:04  tom
 * update sh start_time and duration
 *
 * Revision 1.5  1997/06/02  21:04:55  tom
 * MAXFLOAT
 *
 * Revision 1.4  1997/06/02  20:55:11  tom
 * check quant_scale for Inf
 *
 * Revision 1.3  1996/08/05  14:12:51  tom
 * move scaling from readtensor & rdtensor to this function
 *
 * Revision 1.2  1996/04/24  22:36:52  tom
 * make sure sh6->ecat_calibration_fctr is not zero
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		rdimg6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Get image data from Version 6 file.
 * =============================================================================
 */

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/rdimg6.c,v 1.8 2002/12/17 21:55:15 tom Exp $";

#endif

#include <stdio.h>
#include <stdlib.h>
#include <values.h>
#include <image.h>

float          *rdimg6 (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	Image_subheader *sh6;
	DataSetType     dtype;
	MatrixData     *mdata;
	float          *img = NULL;
	float           scaler;
	short          *simg;
	int             xdim, ydim;
	int             data_type;
	int             matnum;
	int             i;

	dtype = (DataSetType) file->m6->mh->file_type;
	if (dtype != PetVolume && dtype != PetImage) {
		(void) fprintf (stderr, "ERROR [rdimg6]: Unsupported file type %d for %s\n", dtype, file->name);
		return (NULL);
	}
	if (file->m6->mfp == NULL) {
		(void) fprintf (stderr, "ERROR [rdimg6]: Could not get file pointer for %s\n", file->name);
		return (NULL);
	}
	if ((matnum = mat_numcod (indx->frm, indx->pln, indx->gate, indx->data, indx->bed)) == 0) {
		(void) fprintf (stderr, "ERROR [rdimg6]: Could not get matrix number for %s\n", file->name);
		return (NULL);
	}
	mdata = matrix_read (file->m6->mfp, matnum);
	if (mdata == NULL) {
		(void) fprintf (stderr, "ERROR [rdimg6]: Could not read mdata for %s\n", file->name);
		return (NULL);
	}
	file->m6->sh = (Image_subheader *) mdata->shptr;
	sh6 = file->m6->sh;

	if (sh6->frame_duration == 0)
		scaler = ATNSCALER;
	else
		scaler = (float) sh6->frame_duration;
	if (sh6->quant_scale > 0)
		scaler *= sh6->quant_scale;
	if (sh6->ecat_calibration_fctr > 0)
		scaler *= sh6->ecat_calibration_fctr;
	else
		sh6->ecat_calibration_fctr = 1.0;
	if (scaler >= MAXFLOAT)
		scaler = 0.;

	xdim = sh6->dimension_1;
	ydim = sh6->dimension_2;
	data_type = sh6->data_type;
	file->sh->start_time = sh6->frame_start_time;
	file->sh->duration = sh6->frame_duration;

	if ((data_type == SUN_I2) | (data_type == VAX_I2)) {
		img = (float *) malloc (xdim * ydim * sizeof (float));
		if (img == NULL) {
			(void) fprintf (stderr, "ERROR [rdimg6]: Could not malloc memory for img\n");
			return (NULL);
		}
		simg = (short *) mdata->data_ptr;
		for (i = 0; i < xdim * ydim; i++)
			img[i] = scaler * simg[i];
		free (simg);
	} else if (data_type == IEEE_R4) {
		img = (float *) mdata->data_ptr;
		for (i = 0; i < xdim * ydim; i++)
			img[i] *= scaler;
	} else {
		(void) fprintf (stderr, "ERROR [rdimg6]: Unsupported data type in %s plane %d\n",
			 file->name, indx->pln);
		free_matrix_data (mdata);
	}
	return (img);
}
