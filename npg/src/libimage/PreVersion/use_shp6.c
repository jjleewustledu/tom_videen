/*  $Id: use_shp6.c,v 1.4 1999/12/13 17:45:50 tom Exp $
 *  $Log: use_shp6.c,v $
 * Revision 1.4  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.3  1997/12/02  22:45:51  tom
 * get correct time from dynamic image
 *
 * Revision 1.2  1996/07/18  19:22:01  tom
 * set decay_corr to 1
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		use_shp6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Fill SubHeader structure from PETT VI header.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/use_shp6.c,v 1.4 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             use_shp6 (file)
	ImageFile      *file;
{
	SubHeader      *sh;
	PETT6HDR       *p6h;

	if (file->p6 == NULL) {
		(void) fprintf (stderr, "ERROR [use_shp6]: No PETT6HDR defined\n");
		return (ERROR);
	}
	p6h = file->p6;
	if (file->sh == NULL) {
		if ((file->sh = (SubHeader *) malloc (sizeof (SubHeader))) == NULL) {
			(void) fprintf (stderr, "ERROR [use_shp6]: Could not malloc memory for SubHeader\n");
			return (ERROR);
		}
	}
	sh = file->sh;
	sh->number_format = SHORT_DATA;
	sh->file_format = PETT6;
	sh->ndim = 2;
	sh->dim1 = PETT6_DIM;
	sh->dim2 = PETT6_DIM;
	if (p6h->number_of_slices == 0)
		sh->dim3 = 7;
	else
		sh->dim3 = p6h->number_of_slices;
	sh->ring_diff = 0;
	sh->total = 0;
	sh->delayeds = 0;
	sh->multiples = 0;
	sh->trues = 0;
	sh->count_rate = 0;
	sh->scale_factor = 1.;
	sh->calibration_factor = 1.;
	sh->min = 0;
	sh->max = 32767;
	if (p6h->number_of_slices <= 7) {
		sh->sample_size_1 = PETT6_PIXEL_SIZE;
		sh->sample_size_2 = PETT6_PIXEL_SIZE;
		sh->sample_size_3 = PETT6_SLICE_SEP;
	} else if (p6h->number_of_slices == 14) {
		sh->sample_size_1 = PETT6_PIXEL_SIZE;
		sh->sample_size_2 = PETT6_PIXEL_SIZE;
		sh->sample_size_3 = PETT6_SLICE_SEP * 0.5;
	} else {
		sh->sample_size_1 = SMG_PIXEL_SIZE;
		sh->sample_size_2 = SMG_PIXEL_SIZE;
		sh->sample_size_3 = SMG_SLICE_SEP;
	}
	sh->start_time = 0;
	if (p6h->time_per_frame > 0)
		sh->duration = (int) p6h->time_per_frame * 1000.;
	else	
		sh->duration = (int) p6h->scan_time * 1000.;
	sh->deadtime_corr = 1.0;
	sh->decay_corr = 1.0;
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
