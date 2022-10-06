/*  $Id: make_sh.c,v 1.4 2003/03/05 21:18:52 tom Exp $
    $Log: make_sh.c,v $
    Revision 1.4  2003/03/05 21:18:52  tom
    add processing_code

 * Revision 1.3  2000/09/28  14:44:29  tom
 * calloc
 *
 * Revision 1.2  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		make_sh
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Malloc and fill subheader.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/make_sh.c,v 1.4 2003/03/05 21:18:52 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             make_sh (file)
	ImageFile      *file;
{
	SubHeader      *sh;

	if ((file->sh = (SubHeader *) calloc (1, sizeof (SubHeader))) == NULL) {
		(void) fprintf (stderr, "ERROR [make_sh]: cannot calloc SubHeader\n");
		return (ERROR);
	}
	sh = file->sh;
	sh->number_format = 0;
	sh->file_format = 0;
	sh->ndim = 0;
	sh->dim1 = 0;
	sh->dim2 = 0;
	sh->dim3 = 0;
	sh->ring_diff = 0;
	sh->total = 0;
	sh->delayeds = 0;
	sh->multiples = 0;
	sh->trues = 0;
	sh->count_rate = 0;
	sh->scale_factor = 1.;
	sh->calibration_factor = 1.;
	sh->min = 0;
	sh->max = 0;
	sh->deadtime_corr = 0.;
	sh->sample_size_1 = 0;
	sh->sample_size_2 = 0;
	sh->sample_size_3 = 0;
	sh->start_time = 0;
	sh->duration = 0;
	sh->filter_code = 0;
	sh->filter_cutoff_frequency = 0;
	sh->filter_resolution = 0;
	sh->filter_ramp_slope = 0;
	sh->filter_order = 0;
	sh->filter_scatter_fraction = 0;
	sh->filter_scatter_slope = 0;
	sh->processing_code = 0;
	sh->comment = NULL;
	return (OK);
}
