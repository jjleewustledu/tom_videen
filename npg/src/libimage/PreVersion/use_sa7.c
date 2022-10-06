/*  $Id: use_sa7.c,v 1.3 2001/10/15 17:43:38 tom Exp $
    $Log $ */
/* =============================================================================
 *	Function:		use_sa7
 *  Date:           Dec-98
 *  Author:         Tom Videen
 *	Description:	Fill structure sh from Version 7 attencor subheader
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/use_sa7.c,v 1.3 2001/10/15 17:43:38 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             use_sa7 (file)
	ImageFile      *file;
{
	SubHeader      *sh;
	Attn_subheader7 *atn;

	if (file->m7 == NULL) {
		(void) fprintf (stderr, "ERROR [use_sa7]: No version 7 header defined\n");
		return (ERROR);
	}
	if (file->m7->atn == NULL) {
		(void) fprintf (stderr, "ERROR [use_sa7]: No version 7 Attn_subheader defined\n");
		return (ERROR);
	}
	atn = file->m7->atn;
	if (file->sh == NULL) {
		if ((file->sh = (SubHeader *) calloc (1, sizeof (SubHeader))) == NULL) {
			(void) fprintf (stderr, "ERROR [use_sa7]: Could not calloc memory for SubHeader\n");
			return (ERROR);
		}
	}
	sh = file->sh;
	if (atn->data_type == BYTE_TYPE)
		sh->number_format = BYTE_DATA;
	else if (atn->data_type == SUN_I2 || atn->data_type == VAX_I2)
		sh->number_format = SHORT_DATA;
	else if (atn->data_type == IEEE_R4)
		sh->number_format = FLOAT_DATA;
	else
		sh->number_format = SHORT_DATA;
	sh->file_format = MATRIX_REV7;
	sh->ndim = atn->num_dimensions;
	sh->dim1 = atn->num_r_elements;
	sh->dim2 = atn->num_angles;
	sh->dim3 = atn->num_z_elements;
	sh->ring_diff = atn->ring_difference;
	sh->storage_order = atn->storage_order;
	sh->scale_factor = atn->scale_factor;
	sh->calibration_factor = 1.;
	sh->min = atn->attenuation_min;
	sh->max = atn->attenuation_max;
	sh->total = 0;
	sh->delayeds = 0;
	sh->multiples = 0;
	sh->trues = 0;
	sh->sample_size_1 = atn->x_resolution;
	sh->sample_size_2 = atn->y_resolution;
	sh->sample_size_3 = atn->z_resolution;
	sh->start_time = 0;
	sh->duration = 1;
	sh->deadtime_corr = 1.0;
	sh->decay_corr = 0;
	sh->filter_code = 0;
	sh->filter_cutoff_frequency = 0;
	sh->filter_resolution = 0;
	sh->filter_ramp_slope = 0;
	sh->filter_order = 0;
	sh->filter_scatter_fraction = 0;
	sh->filter_scatter_slope = 0;
	sh->comment = NULL;
	return (OK);
}
