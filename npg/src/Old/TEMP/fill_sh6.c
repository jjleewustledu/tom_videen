/*  $Id: fill_sh6.c,v 1.6 2003/03/05 21:18:52 tom Exp $
    $Log: fill_sh6.c,v $
    Revision 1.6  2003/03/05 21:18:52  tom
    add processing_code

 * Revision 1.5  1996/08/15  18:06:16  tom
 * initialize all fields
 *
 * Revision 1.4  1996/08/02  13:56:51  tom
 * remove unused i
 *
 * Revision 1.3  1996/07/18  19:19:45  tom
 * set decay_corr_fctr
 *
 * Revision 1.2  1996/05/14  21:16:20  tom
 * will now leave unaltered any values not defined in SubHeader
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		fill_sh6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Fill Version 6 image subheader from SubHeader structure, sh
 *					Values not in SubHeader structure are not changed.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/fill_sh6.c,v 1.6 2003/03/05 21:18:52 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

int             fill_sh6 (file)
	ImageFile      *file;
{
	SubHeader      *sh;
	Image_subheader *sh6;
	Main_header    *mh6;
	int				i;

	if (file->sh == NULL) {
		fprintf (stderr, "ERROR [fill_sh6]: Subheader is not defined\n");
		return (ERROR);
	}
	if (file->m6 == NULL) {
		fprintf (stderr, "ERROR [fill_sh6]: Version 6 subheader is not defined\n");
		return (ERROR);
	}
	if (file->m6->sh == NULL) {
		if (make_sh6 (file) == ERROR) {
			fprintf (stderr, "ERROR [fill_sh6]: Could not malloc memory for subheader\n");
			return (ERROR);
		}
	}
	sh = file->sh;
	mh6 = file->m6->mh;
	sh6 = file->m6->sh;

	if (sh->number_format == BYTE_DATA)
		sh6->data_type = BYTE_TYPE;
	else if (sh->number_format == SHORT_DATA)
		sh6->data_type = SUN_I2;
	else if (sh->number_format == FLOAT_DATA)
		sh6->data_type = IEEE_R4;
	else
		sh6->data_type = SUN_I2;
	sh6->num_dimensions = sh->ndim;
	sh6->dimension_1 = sh->dim1;
	sh6->dimension_2 = sh->dim2;
	mh6->num_planes = sh->dim3;
	sh6->x_origin = 0;
	sh6->y_origin = 0;
	sh6->recon_scale = 0;
	sh6->quant_scale = sh->scale_factor;
	sh6->image_min = sh->min;
	sh6->image_max = sh->max;
	sh6->pixel_size = sh->sample_size_1;
	sh6->slice_width = sh->sample_size_3;
	sh6->frame_duration = sh->duration;
	sh6->frame_start_time = sh->start_time;
	sh6->filter_code = (short) sh->filter_code;
	sh6->decay_corr_fctr = sh->decay_corr;
	sh6->loss_corr_fctr = 0;
	sh6->intrinsic_tilt = 0;
	sh6->processing_code = sh->processing_code;
	sh6->quant_units = 0;
	sh6->recon_start_day = 0;
	sh6->recon_start_month = 0;
	sh6->recon_start_year = 0;
	sh6->ecat_calibration_fctr = sh->calibration_factor;
	sh6->well_counter_cal_fctr = 0;
	sh6->filter_params[0] = sh->filter_cutoff_frequency;
	sh6->filter_params[1] = sh->filter_resolution;
	sh6->filter_params[2] = sh->filter_ramp_slope;
	sh6->filter_params[3] = sh->filter_order;
	sh6->filter_params[4] = sh->filter_scatter_fraction;
	sh6->filter_params[5] = sh->filter_scatter_slope;
	for (i = 0; i < 40; i++)
		sh6->annotation[i] = '\0';
	return (OK);
}
