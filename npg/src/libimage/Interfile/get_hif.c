/*	$Id$
 *	$Log$
 */
/* =============================================================================
 *  Function:       get_hif
 *  Date:           Nov-95
 *  Author:         John Ollinger
 *  Description:    Get Interfile header.
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

int             get_hif (file)
	ImageFile      *file;
{
	FILE           *fp;
	IFheader       *ifh;
	char            line[MAXLINE], number[20], *string, *hdrfile;
	int             i, bytes_pix, plane, frame, bed = 0;
	float           x;

	*fp = *(file->fp);
	if (fp == NULL) {
		fprintf (stderr, "ERROR [get_hif]: File %s has not been opened\n", file->name);
		return (ERROR);
	}
	*ifh = *(file->ifh);
	if (ifh != NULL)
		free_hif (file);

	ifh = (IFheader *) malloc (sizeof (IFheader));
	ifh->mh = (MainHeader_if *) malloc (sizeof (MainHeader_if));
	ifh->mh->study_date = NULL;
	ifh->mh->sw_revision = NULL;
	ifh->mh->patient_id = NULL;
	ifh->mh->data_file = NULL;

	hdrfile = header_file (file->name);
	if (file->name != NULL) {
		ifh->fname = (char *) malloc (1 + strlen (hdrfile));
		strcpy (ifh->fname, hdrfile);
	}
	if (fgets (line, MAXLINE, fp) == NULL) {
		fprintf (stderr, "ERROR [get_hif]: Cannot read %s\n", file->name);
		return (ERROR);
	} else if (strstr (line, "INTERFILE") == NULL) {
		fprintf (stderr, "ERROR [get_hif]: File %s is not Interfile format\n", file->name);
		return (ERROR);
	}
	plane = -1;
	frame = -1;
	ifh->mh->dim4 = 1;
	while (fgets (line, 200, fp) != NULL) {
		if (strstr (line, "originating system"))
			ifh->mh->orig_system = atoi (getstr (line));
		else if (strstr (line, "number of dimensions"))
			ifh->mh->ndim = atoi (getstr (line));
		else if (strstr (line, "matrix size [1]"))
			ifh->mh->dim1 = atoi (getstr (line));
		else if (strstr (line, "matrix size [2]"))
			ifh->mh->dim2 = atoi (getstr (line));
		else if (strstr (line, "matrix size [3]"))
			ifh->mh->dim3 = atoi (getstr (line));
		else if (strstr (line, "matrix size [4]")) {
			ifh->mh->dim4 = atoi (getstr (line));
			ifh->fh = (FrameHeader_if **) malloc (ifh->mh->dim4 * sizeof (FrameHeader_if *));
			ifh->sh = (SubHeader_if ***) malloc (ifh->mh->dim4 * sizeof (SubHeader_if **));
		} else if (strstr (line, "scaling factor (mm/pixel) [1]"))
			sscanf (getstr (line), "%f", &ifh->mh->voxel_size_1);
		else if (strstr (line, "scaling factor (mm/pixel) [2]"))
			sscanf (getstr (line), "%f", &ifh->mh->voxel_size_2);
		else if (strstr (line, "scaling factor (mm/pixel) [3]"))
			sscanf (getstr (line), "%f", &ifh->mh->voxel_size_3);
		else if (strstr (line, "volume scale factor"))
			sscanf (getstr (line), "%f", &ifh->mh->scale_factor);
		else if (strstr (line, "bed elevation"))
			sscanf (getstr (line), "%f", &ifh->mh->bed_elevation);
		else if (strstr (line, "initial bed position"))
			sscanf (getstr (line), "%f", &ifh->mh->init_bed_pos);
		else if (strstr (line, "number of bed positions"))
			sscanf (getstr (line), "%d", &ifh->mh->nbed);
		else if (strstr (line, "bed offset")) {
			sscanf (getstr (line), "%f", &ifh->mh->bed_off[bed]);
			++bed;
		} else if (strstr (line, "name of data file")) {
			string = getstr (line);
			if (string != NULL) {
				ifh->mh->data_file = (char *) malloc (1 + strlen (string));
				strcpy (ifh->mh->data_file, string);
			}
		} else if (strstr (line, "patient ID")) {
			string = getstr (line);
			if (string != NULL) {
				ifh->mh->patient_id = (char *) malloc (1 + strlen (string));
				strcpy (ifh->mh->patient_id, string);
			}
		} else if (strstr (line, "study date")) {
			string = getstr (line);
			if (string != NULL) {
				ifh->mh->study_date = (char *) malloc (1 + strlen (string));
				strcpy (ifh->mh->study_date, string);
			}
		} else if (strstr (line, "program version")) {
			string = getstr (line);
			if (string != NULL) {
				ifh->mh->sw_revision = (char *) malloc (1 + strlen (string));
				strcpy (ifh->mh->sw_revision, string);
			} else
				ifh->mh->sw_revision = NULL;
		} else if (strstr (line, "number format")) {
			string = getstr (line);
			strcpy (number, string);
		} else if (strstr (line, "number of bytes per pixel"))
			bytes_pix = atoi (getstr (line));
		else if (strstr (line, "frame number")) {
			frame = atoi (getstr (line));
			frame -= 1;
			ifh->sh[frame] = (SubHeader_if **) malloc (ifh->mh->dim3 * sizeof (SubHeader_if *));
			ifh->fh[frame] = (FrameHeader_if *) malloc (sizeof (FrameHeader_if));
			for (i = 0; i < ifh->mh->dim3; i++)
				ifh->sh[frame][i] = NULL;
		} else if (strstr (line, "frame starting time")) {
			if (frame < 0)
				return (ERROR);
			ifh->fh[frame]->starting_time = atoi (getstr (line));
		} else if (strstr (line, "frame duration")) {
			if (frame < 0)
				return (ERROR);
			ifh->fh[frame]->duration = atoi (getstr (line));
		} else if (strstr (line, "plane number")) {
			plane = atoi (getstr (line));
			plane -= 1;
			ifh->sh[frame][plane] = (SubHeader_if *) malloc (sizeof (SubHeader_if));
		} else if (strstr (line, "storage scale factor")) {
			if (frame < 0)
				return (ERROR);
			if (plane < 0)
				return (ERROR);
			sscanf (getstr (line), "%g", &ifh->sh[frame][plane]->storage_sf);
		} else if (strstr (line, "number of trues")) {
			if (frame < 0)
				return (ERROR);
			if (plane < 0)
				return (ERROR);
			ifh->sh[frame][plane]->trues = atoi (getstr (line));
		} else if (strstr (line, "number of randoms")) {
			if (frame < 0)
				return (ERROR);
			if (plane < 0)
				return (ERROR);
			ifh->sh[frame][plane]->randoms = atoi (getstr (line));
		} else if (strstr (line, "number of multiples")) {
			if (frame < 0)
				return (ERROR);
			if (plane < 0)
				return (ERROR);
			ifh->sh[frame][plane]->multiples = atoi (getstr (line));
		} else if (strstr (line, "deadtime correction factor")) {
			if (frame < 0)
				return (ERROR);
			if (plane < 0)
				return (ERROR);
			sscanf (getstr (line), "%g", &ifh->sh[frame][plane]->deadtime_corr);
		} else if (strstr (line, "user defined comment")) {
			string = getstr (line);
			if (string != NULL) {
				ifh->sh[frame][plane]->comment = (char *) malloc (1 + strlen (string));
				strcpy (ifh->sh[frame][plane]->comment, string);
			}
		}
	}

	if (!strcmp (number, "signed integer") && bytes_pix == 1)
		ifh->mh->number_format = BYTE_IF;
	if (!strcmp (number, "signed integer") && bytes_pix == 2)
		ifh->mh->number_format = SHORT_IF;
	else if (!strcmp (number, "float") && bytes_pix == 4)
		ifh->mh->number_format = FLOAT_IF;
	else
		ifh->mh->number_format = UNKNOWN;

	if (ifh->sh == NULL) {
		ifh->fh = (FrameHeader_if **)
			malloc (ifh->mh->dim4 * sizeof (FrameHeader_if *));
		ifh->sh = (SubHeader_if ***)
			malloc (ifh->mh->dim4 * sizeof (SubHeader_if **));
		ifh->sh[0] = (SubHeader_if **) malloc (ifh->mh->dim3 * sizeof (SubHeader_if *));
		for (i = 0; i < ifh->mh->dim3; i++)
			ifh->sh[0][i] = NULL;
		ifh->fh[0] = (FrameHeader_if *) malloc (sizeof (FrameHeader_if));
	}
	for (frame = 0; frame < ifh->mh->dim4; frame++) {
		for (plane = 0; plane < ifh->mh->dim3; plane++) {
			if (ifh->sh[frame][plane] == NULL)
				ifh->sh[frame][plane] = (SubHeader_if *) malloc (sizeof (SubHeader_if));
			ifh->sh[frame][plane]->voxel_size_1 = ifh->mh->voxel_size_1;
			ifh->sh[frame][plane]->voxel_size_2 = ifh->mh->voxel_size_2;
			ifh->sh[frame][plane]->voxel_size_3 = ifh->mh->voxel_size_3;
			ifh->sh[frame][plane]->dim1 = ifh->mh->dim1;
			ifh->sh[frame][plane]->dim2 = ifh->mh->dim2;
			ifh->sh[frame][plane]->dim3 = ifh->mh->dim3;
			ifh->sh[frame][plane]->ndim = ifh->mh->ndim;
			ifh->sh[frame][plane]->number_format = ifh->mh->number_format;
			ifh->sh[frame][plane]->starting_time = ifh->fh[frame]->starting_time;
			ifh->sh[frame][plane]->duration = ifh->fh[frame]->duration;
		}
	}

	free (hdrfile);
	return (OK);
}
