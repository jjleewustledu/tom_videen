/*  $Id: update_sh7.c,v 1.4 1999/12/13 17:45:50 tom Exp $
    $Log: update_sh7.c,v $
    Revision 1.4  1999/12/13 17:45:50  tom
    tov revision

 * Revision 1.3  1996/07/18  19:39:33  tom
 *  sure decay_corr_fctr has a reasonable value
 *
 * Revision 1.2  1996/07/18  19:21:12  tom
 * set decay_corr_fctr
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		update_sh7
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Update select Version 7 image subheader fields using SubHeader 
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/update_sh7.c,v 1.4 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             update_sh7 (file)
	ImageFile      *file;
{
	SubHeader      *sh;
	Image_subheader7 *sh7;

	if (file->sh == NULL) {
		(void) fprintf (stderr, "ERROR [update_sh7]: SubHeader is not defined\n");
		return (ERROR);
	}
	if (file->m7 == NULL) {
		(void) fprintf (stderr, "ERROR [update_sh7]: Version 7 subheader is not defined\n");
		return (ERROR);
	}
	if (file->m7->sh == NULL) {
		(void) fprintf (stderr, "ERROR [update_sh7]: Version 7 subheader is not defined\n");
		return (ERROR);
	}
	sh = file->sh;
	sh7 = file->m7->sh;

	sh7->num_dimensions = sh->ndim;
	sh7->x_dimension = sh->dim1;
	sh7->y_dimension = sh->dim2;
	sh7->z_dimension = sh->dim3;
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
