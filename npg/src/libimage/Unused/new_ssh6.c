/*  $Id$
    $Log$
 */
/* =============================================================================
 *	Function:		new_ssh6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Fill Version 6 scan subheader from structure sh
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

Scan_subheader *new_ssh6 (sh)
	SubHeader      *sh;

{
	Scan_subheader *ssh6;

	if (sh->whole_header != NULL && sh->file_format == MATRIX_REV6)
		ssh6 = (Scan_subheader *) sh->whole_header;
	else
		ssh6 = (Scan_subheader *) malloc (sizeof (Scan_subheader));

	if (sh->number_format == BYTE_DATA)
		ssh6->data_type = BYTE_TYPE;
	else if (sh->number_format == SHORT_DATA)
		ssh6->data_type = SUN_I2;
	else if (sh->number_format == FLOAT_DATA)
		ssh6->data_type = IEEE_R4;
	else
		ssh6->data_type = SUN_I2;
	ssh6->dimension_1 = sh->dim1;
	ssh6->dimension_2 = sh->dim2;
	ssh6->sample_distance = sh->sample_size_1;
	ssh6->scale_factor = sh->scale_factor;
	ssh6->scan_min = sh->min;
	ssh6->scan_max = sh->max;
	ssh6->prompts = sh->total;
	ssh6->delayed = sh->delayeds;
	ssh6->multiples = sh->multiples;
	ssh6->net_trues = sh->trues;
	ssh6->frame_start_time = sh->start_time;
	ssh6->frame_duration = sh->duration;
	ssh6->loss_correction_fctr = sh->deadtime_corr;

	return (ssh6);
}
