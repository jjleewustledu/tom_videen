/*  $Id$
    $Log$
 */
/* =============================================================================
 *	Function:		use_ssh6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Fill structure sh from Version 6 scan subheader
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

SubHeader      *use_ssh6 (ssh6, mh6)
	Scan_subheader *ssh6;
	Main_header    *mh6;

{
	SubHeader      *sh;

	sh = (SubHeader *) malloc (sizeof (SubHeader));

	sh->number_format = ssh6->data_type;
	if (ssh6->data_type == BYTE_TYPE)
		sh->number_format = BYTE_DATA;
	else if (ssh6->data_type == SUN_I2 || ssh6->data_type == VAX_I2)
		sh->number_format = SHORT_DATA;
	else if (ssh6->data_type == IEEE_R4)
		sh->number_format = FLOAT_DATA;
	else
		sh->number_format = SHORT_DATA;
	sh->file_format = MATRIX_REV6;
	sh->ndim = 2;
	sh->dim1 = ssh6->dimension_1;
	sh->dim2 = ssh6->dimension_2;
	sh->dim3 = mh6->num_planes;
	sh->ring_diff = 0;
	sh->total = ssh6->prompts;
	sh->delayeds = ssh6->delayed;
	sh->multiples = ssh6->multiples;
	sh->trues = ssh6->net_trues;
	sh->scale_factor = ssh6->scale_factor;
	sh->min = ssh6->scan_min;
	sh->max = ssh6->scan_max;
	sh->deadtime_corr = ssh6->loss_correction_fctr;
	sh->sample_size_1 = ssh6->sample_distance;
	sh->sample_size_2 = 0.;
	sh->sample_size_3 = 0.;
	sh->start_time = ssh6->frame_start_time;
	sh->duration = ssh6->frame_duration;
	sh->filter_code = 0;
	sh->filter_cutoff_frequency = 0.;
	sh->filter_resolution = 0.;
	sh->filter_ramp_slope = 0.;
	sh->filter_order = 0.;
	sh->filter_scatter_fraction = 0.;
	sh->filter_scatter_slope = 0.;
	sh->comment = NULL;
	sh->whole_header = (void *) ssh6;
	return (sh);
}
