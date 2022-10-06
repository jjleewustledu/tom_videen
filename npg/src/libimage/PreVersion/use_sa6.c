/*  $Id: use_sa6.c,v 1.2 2002/02/26 17:31:49 tom Exp $
    $Log: use_sa6.c,v $
 * Revision 1.2  2002/02/26  17:31:49  tom
 * sample size
 *
 * Revision 1.1  2000/02/18  21:49:13  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		use_sa6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Fill structure atn from Version 6 attenuation subheader
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/use_sa6.c,v 1.2 2002/02/26 17:31:49 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             use_sa6 (file)
	ImageFile      *file;
{
	SubHeader      *sh;
	Attn_subheader *sa6;
	Main_header    *mh6;

	if (file->m6 == NULL) {
		(void) fprintf (stderr, "ERROR [use_sa6]: Version 6 header is not defined\n");
		return (ERROR);
	}
	sa6 = file->m6->atn;
	mh6 = file->m6->mh;
	if (sa6 == NULL) {
		(void) fprintf (stderr, "ERROR [use_sa6]: Version 6 subheader is not defined\n");
		return (ERROR);
	}
	if (file->sh == NULL) {
		if ((file->sh = (SubHeader *) malloc (sizeof (SubHeader))) == NULL) {
			(void) fprintf (stderr, "ERROR [use_sa6]: Could not malloc memory for SubHeader\n");
			return (ERROR);
		}
	}
	sh = file->sh;
	if (sa6->data_type == BYTE_TYPE)
		sh->number_format = BYTE_DATA;
	else if (sa6->data_type == SUN_I2 || sa6->data_type == VAX_I2)
		sh->number_format = SHORT_DATA;
	else if (sa6->data_type == IEEE_R4)
		sh->number_format = FLOAT_DATA;
	else
		sh->number_format = SHORT_DATA;
	sh->file_format = MATRIX_REV6;
	sh->ndim = 2;
	sh->dim1 = sa6->dimension_1;
	sh->dim2 = sa6->dimension_2;
	sh->dim3 = mh6->num_planes;
	sh->scale_factor = sa6->scale_factor;
	sh->calibration_factor = 1;
	sh->min = 0;
	sh->max = 0;
	sh->deadtime_corr = 1;
	sh->sample_size_1 = sa6->sample_distance;
	sh->sample_size_2 = sa6->y_radius;
	sh->sample_size_3 = 1;
	sh->start_time = 0;
	sh->duration = 0;
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
