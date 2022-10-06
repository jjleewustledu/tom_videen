/*	$Id$
 *	$Log$
 */
/* =============================================================================
 *  Function:       make_hif
 *  Date:           Nov-95
 *  Author:         John Ollinger
 *  Description:    Malloc an Interfile header.
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

int             make_hif (file, dim3, dim4)
	ImageFile      *file;
	int             dim3, dim4;
{
	IFheader       *ifh;
	int             plane, frame;

	ifh = (IFheader *) malloc (sizeof (IFheader));
	ifh->mh = (MainHeader_if *) malloc (sizeof (MainHeader_if));
	ifh->mh->study_date = NULL;
	ifh->mh->sw_revision = NULL;
	ifh->mh->patient_id = NULL;
	ifh->mh->data_file = NULL;
	ifh->fh = (FrameHeader_if **) malloc (dim4 * sizeof (FrameHeader_if *));
	ifh->sh = (SubHeader_if ***) malloc (dim4 * sizeof (SubHeader_if **));
	for (frame = 0; frame < dim4; frame++) {
		ifh->fh[frame] = (FrameHeader_if *) malloc (sizeof (FrameHeader_if));
		ifh->sh[frame] = (SubHeader_if **) malloc (dim3 * sizeof (SubHeader_if *));
		for (plane = 0; plane < dim3; plane++) {
			ifh->sh[frame][plane] = (SubHeader_if *) malloc (sizeof (SubHeader_if));
			ifh->sh[frame][plane]->comment = NULL;
		}
	}
	return (OK);
}
