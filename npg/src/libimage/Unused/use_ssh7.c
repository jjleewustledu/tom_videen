/*  $Id$
    $Log$
 */
/* =============================================================================
 *	Function:		use_ssh7
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Fill structure sh from Version 7 scan subheader
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/petsun19/tom/src/libimg/RCS/cti.c,v 1.15 1996/03/12 22:43:02 tom Exp $";
#endif

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <libimg.h>
#include <nr.h>
#include <nrutil.h>

SubHeader      *use_ssh7 (ssh7)
	Scan3D_subheader7 *ssh7;

{
	int             i;
	SubHeader      *sh;

	sh = (SubHeader *) malloc (sizeof (SubHeader));

	if (ssh7->data_type == BYTE_TYPE)
		sh->number_format = BYTE_DATA;
	else if (ssh7->data_type == SUN_I2 || ssh7->data_type == VAX_I2)
		sh->number_format = SHORT_DATA;
	else if (ssh7->data_type == IEEE_R4)
		sh->number_format = FLOAT_DATA;
	else
		sh->number_format = SHORT_DATA;
	sh->file_format = MATRIX_REV7;
	sh->ndim = ssh7->num_dimensions;
	sh->dim1 = ssh7->num_r_elements;
	sh->dim2 = ssh7->num_angles;
	for (i = 0, sh->dim3 = 0; i < ssh7->ring_difference; i++)
		sh->dim3 += ssh7->num_z_elements[i];
	sh->ring_diff = ssh7->ring_difference;
	sh->total = ssh7->prompts;
	sh->delayeds = ssh7->delayed;
	sh->multiples = ssh7->multiples;
	sh->trues = ssh7->net_trues;
	sh->scale_factor = ssh7->scale_factor;
	sh->min = ssh7->scan_min;
	sh->max = ssh7->scan_max;
	sh->deadtime_corr = ssh7->loss_correction_fctr;
	sh->sample_size_1 = ssh7->x_resolution;
	sh->sample_size_2 = ssh7->y_resolution;
	sh->sample_size_3 = ssh7->z_resolution;
	sh->start_time = ssh7->frame_start_time;
	sh->duration = ssh7->frame_duration;
	sh->filter_code = 0;
	sh->filter_cutoff_frequency = 0.;
	sh->filter_resolution = 0.;
	sh->filter_ramp_slope = 0.;
	sh->filter_order = 0.;
	sh->filter_scatter_fraction = 0.;
	sh->filter_scatter_slope = 0.;
	sh->comment = NULL;
	sh->whole_header = ssh7->whole_header;

	return (sh);
}
