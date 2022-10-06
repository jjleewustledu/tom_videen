/*  $Id: use_ss7.c,v 1.7 2001/11/21 17:01:28 tom Exp $
    $Log: use_ss7.c,v $
 * Revision 1.7  2001/11/21  17:01:28  tom
 * elim param
 *
 * Revision 1.6  2001/09/26  14:48:51  tom
 * get number of z-elements from first frame
 *
 * Revision 1.5  2000/09/27  19:03:28  tom
 * *** empty log message ***
 *
 * Revision 1.4  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.3  1998/12/15  14:52:22  tom
 * debugged version
 *
 * Revision 1.2  1998/12/11  17:33:26  tom
 * fix bug
 *
 * Revision 1.1  1998/12/10  18:42:01  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		use_ss7
 *  Date:           Dec-98
 *  Author:         Tom Videen
 *	Description:	Fill structure sh from Version 7 scan subheader
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/use_ss7.c,v 1.7 2001/11/21 17:01:28 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             use_ss7 (file)
	ImageFile      *file;
{
	SubHeader      *sh;
	Scan3D_subheader7 *s3d;

	if (file->m7 == NULL) {
		(void) fprintf (stderr, "ERROR [use_ss7]: No version 7 header defined\n");
		return (ERROR);
	}
	if (file->m7->s3d == NULL) {
		(void) fprintf (stderr, "ERROR [use_ss7]: No version 7 Scan3D_subheader defined\n");
		return (ERROR);
	}
	s3d = file->m7->s3d;
	if (file->sh == NULL) {
		if ((file->sh = (SubHeader *) calloc (1, sizeof (SubHeader))) == NULL) {
			(void) fprintf (stderr, "ERROR [use_ss7]: Could not calloc memory for SubHeader\n");
			return (ERROR);
		}
	}
	sh = file->sh;
	if (s3d->data_type == BYTE_TYPE)
		sh->number_format = BYTE_DATA;
	else if (s3d->data_type == SUN_I2 || s3d->data_type == VAX_I2)
		sh->number_format = SHORT_DATA;
	else if (s3d->data_type == IEEE_R4)
		sh->number_format = FLOAT_DATA;
	else
		sh->number_format = SHORT_DATA;
	sh->file_format = MATRIX_REV7;
	sh->ndim = s3d->num_dimensions;
	sh->dim1 = s3d->num_r_elements;
	sh->dim2 = s3d->num_angles;
	sh->dim3 = s3d->num_z_elements[0];
	sh->ring_diff = s3d->ring_difference;
	sh->storage_order = s3d->storage_order;
	sh->scale_factor = s3d->scale_factor;
	sh->calibration_factor = 1.;
	sh->min = s3d->scan_min;
	sh->max = s3d->scan_max;
	sh->total = s3d->prompts;
	sh->delayeds = s3d->delayed;
	sh->multiples = s3d->multiples;
	sh->trues = s3d->net_trues;
	sh->sample_size_1 = s3d->x_resolution;
	sh->sample_size_2 = s3d->v_resolution;
	sh->sample_size_3 = s3d->z_resolution;
	sh->start_time = s3d->frame_start_time;
	sh->duration = s3d->frame_duration;
	sh->deadtime_corr = 1.0;
	sh->decay_corr = s3d->deadtime_correction_fctr;
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
