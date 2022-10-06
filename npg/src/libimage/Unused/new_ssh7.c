/*  $Id$
    $Log$
 */
/* =============================================================================
 *	Function:		new_ssh7
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Fill Version 7 scan subheader from structure sh
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

Scan3D_subheader7 *new_ssh7 (sh)
	SubHeader      *sh;

{
	Scan3D_subheader7 *ssh7;
	short          *hdr, *hdr0;
	int             i;

	ssh7 = (Scan3D_subheader7 *) malloc (sizeof (Scan3D_subheader7));
	if (sh->whole_header != NULL && sh->file_format == MATRIX_REV7) {
		hdr = (short *) ssh7;
		hdr0 = (short *) sh->whole_header;
		for (i = 0; i < LENREC_S; i++)
			hdr[i] = hdr0[i];
	}
	if (sh->number_format == BYTE_DATA)
		ssh7->data_type = BYTE_TYPE;
	else if (sh->number_format == SHORT_DATA)
		ssh7->data_type = SUN_I2;
	else if (sh->number_format == FLOAT_DATA)
		ssh7->data_type = IEEE_R4;
	else
		ssh7->data_type = SUN_I2;
	ssh7->num_dimensions = sh->ndim;
	ssh7->num_r_elements = sh->dim1;
	ssh7->num_angles = sh->dim2;
	ssh7->num_z_elements[0] = sh->dim3;
	ssh7->ring_difference = sh->ring_diff;
	ssh7->x_resolution = sh->sample_size_1;
	ssh7->y_resolution = sh->sample_size_2;
	ssh7->z_resolution = sh->sample_size_3;
	ssh7->scale_factor = sh->scale_factor;
	ssh7->scan_min = sh->min;
	ssh7->scan_max = sh->max;
	ssh7->prompts = sh->total;
	ssh7->delayed = sh->delayeds;
	ssh7->multiples = sh->multiples;
	ssh7->net_trues = sh->trues;
	ssh7->frame_start_time = sh->start_time;
	ssh7->frame_duration = sh->duration;
	ssh7->loss_correction_fctr = sh->deadtime_corr;
	ssh7->whole_header = sh->whole_header;
	return (ssh7);
}
