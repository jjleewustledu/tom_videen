/*  $Id: use_shif.c,v 1.2 2000/09/28 14:44:29 tom Exp $
 *  $Log: use_shif.c,v $
 *  Revision 1.2  2000/09/28 14:44:29  tom
 *  calloc
 *
 * Revision 1.1  2000/07/18  15:40:41  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		use_shif
 *  Date:           Jul 2000
 *  Author:         Tom Videen
 *	Description:	Fill SubHeader structure from Interfile header.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/use_shif.c,v 1.2 2000/09/28 14:44:29 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             use_shif (file)
	ImageFile      *file;
{
	SubHeader      *sh;
	MainHeader_if  *ifh;

	if (file->ih == NULL) {
		(void) fprintf (stderr, "ERROR [use_shif]: No Interfile defined\n");
		return (ERROR);
	}
	if (file->ih->mh == NULL) {
		(void) fprintf (stderr, "ERROR [use_mhif]: No Interfile header defined\n");
		return (ERROR);
	}
	ifh = (MainHeader_if *) file->ih->mh;
	if (file->sh == NULL) {
		if ((file->sh = (SubHeader *) calloc (1, sizeof (SubHeader))) == NULL) {
			(void) fprintf (stderr, "ERROR [use_shif]: Could not calloc memory for SubHeader\n");
			return (ERROR);
		}
	}
	sh = file->sh;
	sh->number_format = ifh->number_format;
	sh->file_format = INTERFILE;
	sh->ndim = ifh->ndim;
	sh->dim1 = ifh->dim1;
	sh->dim2 = ifh->dim2;
	sh->dim3 = ifh->dim3;
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
	sh->sample_size_1 = ifh->voxel_size_1;
	sh->sample_size_2 = ifh->voxel_size_2;
	sh->sample_size_3 = ifh->voxel_size_3;
	sh->start_time = 0;
	sh->duration = 0;
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
