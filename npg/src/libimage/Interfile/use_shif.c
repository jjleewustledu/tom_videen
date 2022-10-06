/*	$Id$
 *	$Log$
 */
/* =============================================================================
 *  Function:       use_shif
 *  Date:           Nov-95
 *  Author:         John Ollinger
 *  Description:    Fill subheader using Interfile subheader.
 *  History:
 *      Mar-96  TOV Modified name, return value, and calling parameters.
 *
 *	Copyright 1991-1996, Washington University.
 *	All rights reserved. Unauthorized reproduction prohibited.
 * =============================================================================
 */

#ifndef lint
static char     rcsid[] = "$Header$";

#endif

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <image.h>
#include <libimage.h>

int             use_shif (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	SubHeader_if   *ifsh;
	SubHeader      *sh;
	int             frame, plane;
	int             i;

	frame = indx->frm - 1;
	plane = indx->pln - 1;
	*ifsh = *(file->ifh->sh[frame][plane]);
	if (ifsh == NULL) {
		fprintf (stderr, "ERROR [use_shif]: Interfile subheader undefined for frame %d plane %d\n",
			 frame, plane);
		return (ERROR);
	}
	*sh = *(file->sh);
	if (sh == NULL) {
		if ((sh = (SubHeader *) malloc (sizeof (SubHeader))) == NULL) {
			fprintf (stderr, "ERROR [use_shif]: Cannot malloc memory for sh\n");
			return (ERROR);
		}
	}
	sh->number_format = ifsh->number_format;
	sh->file_format = INTERFILE;
	sh->ndim = ifsh->ndim;
	sh->dim1 = ifsh->dim1;
	sh->dim2 = ifsh->dim2;
	sh->dim3 = ifsh->dim3;

	sh->delayeds = ifsh->randoms;
	sh->multiples = ifsh->multiples;
	sh->trues = ifsh->trues;

	sh->scale_factor = ifsh->storage_sf;
	sh->calibration_factor = 1.;

	sh->deadtime_corr = ifsh->deadtime_corr;
	sh->sample_size_1 = ifsh->voxel_size_1;
	sh->sample_size_2 = ifsh->voxel_size_2;
	sh->sample_size_3 = ifsh->voxel_size_3;
	sh->start_time = ifsh->starting_time;
	sh->duration = ifsh->duration;

	if (ifsh->comment != NULL) {
		sh->comment = (char *) malloc (1 + strlen (ifsh->comment));
		strcpy (sh->comment, ifsh->comment);
	} else
		strcpy (sh->comment, NULL);

	return (OK);
}
