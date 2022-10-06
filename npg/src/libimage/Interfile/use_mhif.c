/*	$Id$
 *	$Log$
 */
/* =============================================================================
 *  Function:       use_mhif
 *  Date:           Nov-95
 *  Author:         John Ollinger
 *  Description:    Fill mainheader using Interfile mainheader.
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

int             use_mhif (file)
	ImageFile      *file;
{
	MainHeader_if  *ifmh;
	MainHeader     *mh;
	int             i, j;

	*ifmh = *(file->ifh->mh);
	if (ifmh == NULL) {
		fprintf (stderr, "ERROR [use_hif]: No Interfile data for %s\n", file->name);
		return (ERROR);
	}
	*mh = *(file->mh);
	if (mh == NULL)
		mh = (MainHeader *) malloc (sizeof (MainHeader));

	if (ifmh->data_file != NULL) {
		mh->data_file = (char *) malloc (1 + strlen (ifmh->data_file));
		strcpy (mh->data_file, ifmh->data_file);
	} else
		mh->data_file = NULL;

	mh->file_format = INTERFILE;
	mh->number_format = ifmh->number_format;

	mh->filetype = ifmh->file_type;
	mh->nplane = ifmh->dim3;
	mh->ngate = 1;
	if (ifmh->nbed == 1) {
		mh->nframe = ifmh->dim4;
		mh->nbed = ifmh->nbed;
	} else {
		mh->nframe = 1;
		mh->nbed = ifmh->dim4;
	}
	mh->scanner_type = ifmh->orig_system;
	if (ifmh->study_date != NULL)
		strcpy (mh->date_time, ifmh->study_date);
	mh->start_time = -1;

	if (ifmh->patient_id != NULL)
		strcpy (mh->patnum, ifmh->patient_id);
	mh->bed_elevation = ifmh->bed_elevation;
	mh->init_bed_pos = ifmh->init_bed_pos;
	for (i = 0; i < 16; i++)
		mh->bed_off[i] = ifmh->bed_off[i];
	mh->slicesep = ifmh->voxel_size_3;
	mh->calibration_factor = ifmh->scale_factor;

	if (ifmh->sw_revision != NULL) {
		mh->sw_revision = (char *) malloc (strlen (ifmh->sw_revision) + 1);
		strcpy (mh->sw_revision, ifmh->sw_revision);
	} else
		mh->sw_revision = NULL;
	return (OK);
}
