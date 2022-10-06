/*	$Id$
 *	$Log$
 */
/* =============================================================================
 *  Function:       make_shif
 *  Date:           Nov-95
 *  Author:         John Ollinger
 *  Description:    Fill Interfile mainheader using mainheader.
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

int             make_shif (file)
	ImageFile      *file;
{
	SubHeader      *sh;
	SubHeader_if   *ifsh;
	int             i;

	*sh = *(file->sh);
	if (sh == NULL) {
		fprintf (stderr, "ERROR [make_shif]: No SubHeader data for %s\n", file->name);
		return (ERROR);
	}
	*ifsh = *(file->ifh->sh);
	if (*ifsh == NULL)
		ifsh = (SubHeader_if *) malloc (sizeof (SubHeader_if));

	ifsh->number_format = sh->number_format;
	ifsh->ndim = sh->ndim;
	ifsh->dim1 = sh->dim1;
	ifsh->dim2 = sh->dim2;
	ifsh->dim3 = sh->dim3;
	ifsh->voxel_size_1 = sh->sample_size_1;
	ifsh->voxel_size_2 = sh->sample_size_2;
	ifsh->voxel_size_3 = sh->sample_size_3;
	ifsh->storage_sf = sh->scale_factor * sh->calibration_factor;
	ifsh->trues = sh->total;
	ifsh->randoms = sh->delayeds;
	ifsh->multiples = sh->multiples;
	ifsh->starting_time = sh->start_time;
	ifsh->duration = sh->duration;
	ifsh->deadtime_corr = sh->deadtime_corr;
	if (sh->comment != NULL) {
		ifsh->comment = (char *) malloc (1 + strlen (sh->comment));
		strcpy (ifsh->comment, sh->comment);
	} else
		ifsh->comment = NULL;
	return (OK);
}
