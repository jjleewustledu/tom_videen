/*  $Id: use_sh7.c,v 1.5 2003/01/23 15:16:58 tom Exp $
    $Log: use_sh7.c,v $
    Revision 1.5  2003/01/23 15:16:58  tom
    add processing_code

 * Revision 1.4  2000/09/27  19:03:28  tom
 * *** empty log message ***
 *
 * Revision 1.3  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.2  1996/07/18  19:21:44  tom
 * set decay_corr
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		use_sh7
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Fill structure sh from Version 7 image subheader
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/use_sh7.c,v 1.5 2003/01/23 15:16:58 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             use_sh7 (file)
	ImageFile      *file;
{
	SubHeader      *sh;
	Image_subheader7 *sh7;

	if (file->m7 == NULL) {
		(void) fprintf (stderr, "ERROR [use_sh7]: No version 7 header defined\n");
		return (ERROR);
	}
	if (file->m7->sh == NULL) {
		(void) fprintf (stderr, "ERROR [use_sh7]: No version 7 Image_subheader defined\n");
		return (ERROR);
	}
	sh7 = file->m7->sh;
	if (file->sh == NULL) {
		if ((file->sh = (SubHeader *) calloc (1, sizeof (SubHeader))) == NULL) {
			(void) fprintf (stderr, "ERROR [use_sh7]: Could not calloc memory for SubHeader\n");
			return (ERROR);
		}
	}
	sh = file->sh;
	if (sh7->data_type == BYTE_TYPE)
		sh->number_format = BYTE_DATA;
	else if (sh7->data_type == SUN_I2 || sh7->data_type == VAX_I2)
		sh->number_format = SHORT_DATA;
	else if (sh7->data_type == IEEE_R4)
		sh->number_format = FLOAT_DATA;
	else
		sh->number_format = SHORT_DATA;
	sh->file_format = MATRIX_REV7;
	sh->ndim = sh7->num_dimensions;
	sh->dim1 = sh7->x_dimension;
	sh->dim2 = sh7->y_dimension;
	sh->dim3 = sh7->z_dimension;
	sh->scale_factor = sh7->scale_factor;
	sh->calibration_factor = 1.;
	sh->min = sh7->image_min;
	sh->max = sh7->image_max;
	sh->sample_size_1 = sh7->x_pixel_size;
	sh->sample_size_2 = sh7->y_pixel_size;
	sh->sample_size_3 = sh7->z_pixel_size;
	sh->start_time = sh7->frame_start_time;
	sh->duration = sh7->frame_duration;
	sh->deadtime_corr = 1.0;
	sh->decay_corr = sh7->decay_corr_fctr;
	sh->filter_code = (int) sh7->filter_code;
	sh->filter_cutoff_frequency = sh7->filter_cutoff_frequency;
	sh->filter_resolution = sh7->filter_resolution;
	sh->filter_ramp_slope = sh7->filter_ramp_slope;
	sh->filter_order = sh7->filter_order;
	sh->filter_scatter_fraction = sh7->filter_scatter_fraction;
	sh->filter_scatter_slope = sh7->filter_scatter_slope;
	sh->processing_code = sh7->processing_code;
	sh->comment = NULL;
	return (OK);
}
