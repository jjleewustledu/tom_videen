/*  $Id: use_sh6.c,v 1.6 2003/03/05 21:18:52 tom Exp $
    $Log: use_sh6.c,v $
    Revision 1.6  2003/03/05 21:18:52  tom
    add processing_code

 * Revision 1.5  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.4  1996/07/18  19:21:29  tom
 * set decay_corr
 *
 * Revision 1.3  1996/04/24  22:47:04  tom
 * make sure ecat_calibration_fctr > 0
 *
 * Revision 1.2  1996/04/24  22:16:50  tom
 * check that sh->calibration_factor is not zero
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		use_sh6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Fill structure sh from Version 6 image subheader
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/use_sh6.c,v 1.6 2003/03/05 21:18:52 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             use_sh6 (file)
	ImageFile      *file;
{
	SubHeader      *sh;
	Image_subheader *sh6;
	Main_header    *mh6;

	if (file->m6 == NULL) {
		(void) fprintf (stderr, "ERROR [use_sh6]: Version 6 header is not defined\n");
		return (ERROR);
	}
	sh6 = file->m6->sh;
	mh6 = file->m6->mh;
	if (sh6 == NULL) {
		(void) fprintf (stderr, "ERROR [use_sh6]: Version 6 subheader is not defined\n");
		return (ERROR);
	}
	if (file->sh == NULL) {
		if ((file->sh = (SubHeader *) malloc (sizeof (SubHeader))) == NULL) {
			(void) fprintf (stderr, "ERROR [use_sh6]: Could not malloc memory for SubHeader\n");
			return (ERROR);
		}
	}
	sh = file->sh;
	if (sh6->data_type == BYTE_TYPE)
		sh->number_format = BYTE_DATA;
	else if (sh6->data_type == SUN_I2 || sh6->data_type == VAX_I2)
		sh->number_format = SHORT_DATA;
	else if (sh6->data_type == IEEE_R4)
		sh->number_format = FLOAT_DATA;
	else
		sh->number_format = SHORT_DATA;
	sh->file_format = MATRIX_REV6;
	sh->ndim = sh6->num_dimensions;
	sh->dim1 = sh6->dimension_1;
	sh->dim2 = sh6->dimension_2;
	sh->dim3 = mh6->num_planes;
	if (sh->dim3 == 0)
		sh->dim3 = 31;
	sh->scale_factor = sh6->quant_scale;
	if (sh6->ecat_calibration_fctr <= 0)
		sh6->ecat_calibration_fctr = 1.0;
	sh->calibration_factor = sh6->ecat_calibration_fctr;
	sh->min = sh6->image_min;
	sh->max = sh6->image_max;
	sh->deadtime_corr = sh6->loss_corr_fctr;
	sh->sample_size_1 = sh6->pixel_size;
	sh->sample_size_2 = sh6->pixel_size;
	sh->sample_size_3 = sh6->slice_width;
	if (sh->sample_size_3 == 0)
		sh->sample_size_3 = 0.3375;
	sh->start_time = sh6->frame_start_time;
	sh->duration = sh6->frame_duration;
	sh->decay_corr = sh6->decay_corr_fctr;
	sh->filter_code = (int) sh6->filter_code;
	sh->filter_cutoff_frequency = sh6->filter_params[0];
	sh->filter_resolution = sh6->filter_params[1];
	sh->filter_ramp_slope = sh6->filter_params[2];
	sh->filter_order = sh6->filter_params[3];
	sh->filter_scatter_fraction = sh6->filter_params[4];
	sh->filter_scatter_slope = sh6->filter_params[5];
	sh->processing_code = sh6->processing_code;
	sh->comment = NULL;
	return (OK);
}
