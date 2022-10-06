/*  $Id: update_sh6.c,v 1.3 1999/12/13 17:45:50 tom Exp $
    $Log: update_sh6.c,v $
    Revision 1.3  1999/12/13 17:45:50  tom
    tov revision

 * Revision 1.2  1996/07/18  19:21:01  tom
 * set decay_corr_fctr
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		update_sh6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Update select Version 6 image subheader fields using SubHeader
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/update_sh6.c,v 1.3 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             update_sh6 (file)
	ImageFile      *file;
{
	SubHeader      *sh;
	Image_subheader *sh6;
	Main_header    *mh6;

	if (file->sh == NULL) {
		(void) fprintf (stderr, "ERROR [update_sh6]: Subheader is not defined\n");
		return (ERROR);
	}
	if (file->m6 == NULL) {
		(void) fprintf (stderr, "ERROR [update_sh6]: Version 6 subheader is not defined\n");
		return (ERROR);
	}
	if (file->m6->sh == NULL) {
		(void) fprintf (stderr, "ERROR [update_sh6]: Version 6 subheader is not defined\n");
		return (ERROR);
	}
	sh = file->sh;
	mh6 = file->m6->mh;
	sh6 = file->m6->sh;

	sh6->num_dimensions = sh->ndim;
	sh6->dimension_1 = sh->dim1;
	sh6->dimension_2 = sh->dim2;
	mh6->num_planes = sh->dim3;
	sh6->pixel_size = sh->sample_size_1;
	sh6->pixel_size = sh->sample_size_2;
	sh6->slice_width = sh->sample_size_3;
	sh6->frame_start_time = sh->start_time;
	sh6->frame_duration = sh->duration;
	sh6->decay_corr_fctr = sh->decay_corr;
	sh6->filter_code = (short) sh->filter_code;
	sh6->filter_params[0] = sh->filter_cutoff_frequency;
	sh6->filter_params[1] = sh->filter_resolution;
	sh6->filter_params[2] = sh->filter_ramp_slope;
	sh6->filter_params[3] = sh->filter_order;
	sh6->filter_params[4] = sh->filter_scatter_fraction;
	sh6->filter_params[5] = sh->filter_scatter_slope;
	return (OK);
}
