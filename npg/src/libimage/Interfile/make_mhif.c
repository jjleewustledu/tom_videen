/*	$Id$
 *	$Log$
 */
/* =============================================================================
 *  Function:       make_mhif
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

int             make_mhif (file)
	ImageFile      *file;
{
	IFheader       *ifh;
	MainHeader_if  *ifmh;
	MainHeader     *mh;
	int             i, j;

	*mh = *(file->mh);
	if (mh == NULL) {
		fprintf (stderr, "ERROR [make_mhif]: No MainHeader data for %s\n", file->name);
		return (ERROR);
	}
	*ifh = *(file->ifh);
	if (ifh != NULL)
		free_mhif (file);

	ifh = (IFheader *) malloc (sizeof (IFheader));
	*ifmh = *(file->ifh->mh);
	ifmh = (MainHeader_if *) malloc (sizeof (MainHeader_if));
	ifmh->data_file = NULL;

	if (mh->data_file != NULL) {
		ifmh->data_file = (char *) malloc (1 + strlen (mh->data_file));
		strcpy (ifmh->data_file, mh->data_file);
	}
	ifmh->number_format = mh->number_format;
	ifmh->orig_system = mh->scanner_type;
	ifmh->file_type = mh->filetype;
	if (mh->date_time != NULL) {
		ifmh->study_date = (char *) malloc (1 + strlen (mh->date_time));
		strcpy (ifmh->study_date, mh->date_time);

	} else
		ifmh->study_date = NULL;

	if (mh->patnum != NULL) {
		ifmh->patient_id = (char *) malloc (1 + strlen (mh->patnum));
		strcpy (ifmh->patient_id, mh->patnum);
	} else
		ifmh->patient_id = NULL;

	if (mh->sw_revision != NULL) {
		ifmh->sw_revision = (char *) malloc (strlen (mh->sw_revision) + 1);
		strcpy (ifmh->sw_revision, mh->sw_revision);
	} else
		ifmh->sw_revision = NULL;

	ifmh->dim3 = mh->nplane;
	if (mh->nframe > 1)
		ifmh->dim4 = mh->nframe;
	else if (mh->nbed > 0)
		ifmh->dim4 = mh->nbed;
	else
		ifmh->dim4 = 1;

	if (ifmh->dim3 > 1 && ifmh->dim4 > 1)
		ifmh->ndim = 4;
	else if (ifmh->dim3 > 1 || ifmh->dim4 > 1)
		ifmh->ndim = 3;
	else
		ifmh->ndim = 2;

	ifmh->nbed = mh->nbed;
	ifmh->bed_elevation = mh->bed_elevation;
	ifmh->init_bed_pos = mh->init_bed_pos;
	for (i = 0, j = 0; i < 16; i++)
		ifmh->bed_off[i] = mh->bed_off[i];
	ifmh->voxel_size_3 = mh->slicesep;
	ifmh->scale_factor = mh->calibration_factor;
	return (OK);
}
