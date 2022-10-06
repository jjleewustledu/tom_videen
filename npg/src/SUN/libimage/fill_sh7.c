/*  $Id: fill_sh7.c,v 1.4 2000/09/27 19:03:28 tom Exp $
    $Log: fill_sh7.c,v $
    Revision 1.4  2000/09/27 19:03:28  tom
    *** empty log message ***

 * Revision 1.3  1996/07/18  19:38:30  tom
 * make sure decay_corr_fctr has a reasonable value
 *
 * Revision 1.2  1996/07/18  19:20:42  tom
 * set decay_corr_fctr
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		fill_sh7
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Fill Version 7 image subheader from SubHeader structure, sh.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/fill_sh7.c,v 1.4 2000/09/27 19:03:28 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

int             fill_sh7 (file)
	ImageFile      *file;
{
	SubHeader      *sh;
	Image_subheader7 *sh7;

	if (file->sh == NULL) {
		fprintf (stderr, "ERROR [fill_sh7]: SubHeader is not defined\n");
		return (ERROR);
	}
	if (file->m7 == NULL) {
		fprintf (stderr, "ERROR [fill_sh7]: Version 7 subheader is not defined\n");
		return (ERROR);
	}
	if (file->m7->sh == NULL) {
		if (make_sh7 (file) == ERROR) {
			fprintf (stderr, "ERROR [fill_sh7]: Could not calloc memory for subheader\n");
			return (ERROR);
		}
	}
	sh = file->sh;
	sh7 = file->m7->sh;

	if (sh->number_format == BYTE_DATA)
		sh7->data_type = BYTE_TYPE;
	else if (sh->number_format == SHORT_DATA)
		sh7->data_type = SUN_I2;
	else if (sh->number_format == FLOAT_DATA)
		sh7->data_type = IEEE_R4;
	else
		sh7->data_type = SUN_I2;

	sh7->num_dimensions = sh->ndim;
	sh7->x_dimension = sh->dim1;
	sh7->y_dimension = sh->dim2;
	sh7->z_dimension = sh->dim3;
	sh7->scale_factor = sh->scale_factor;
	sh7->image_min = sh->min;
	sh7->image_max = sh->max;
	sh7->x_pixel_size = sh->sample_size_1;
	sh7->y_pixel_size = sh->sample_size_2;
	sh7->z_pixel_size = sh->sample_size_3;
	sh7->frame_start_time = sh->start_time;
	sh7->frame_duration = sh->duration;
	sh7->decay_corr_fctr = sh->decay_corr;
	if (sh7->decay_corr_fctr <= 1.0)
		sh7->decay_corr_fctr = 1.0;
	sh7->filter_code = (short) sh->filter_code;
	sh7->filter_cutoff_frequency = sh->filter_cutoff_frequency;
	sh7->filter_resolution = sh->filter_resolution;
	sh7->filter_ramp_slope = sh->filter_ramp_slope;
	sh7->filter_order = sh->filter_order;
	sh7->filter_scatter_fraction = sh->filter_scatter_fraction;
	sh7->filter_scatter_slope = sh->filter_scatter_slope;
	return (OK);
}
