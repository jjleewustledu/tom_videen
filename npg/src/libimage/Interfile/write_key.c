/*	$Id$
 *	$Log$
 */
/* =============================================================================
 *  Function:       write_key
 *  Date:           Nov-95
 *  Author:         John Ollinger
 *  Description:    Write Interfile key.
 *  History:
 *      Mar-96  TOV Modified
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

int             write_key (fpw, line, ifh, bed)
	FILE           *fpw;
	char           *line;
	IFheader       *ifh;
	int             bed;
{
	char           *filename;

	if (strstr (line, "originating system"))
		fprintf (fpw, "originating system := %d\n", ifh->mh->orig_system);
	else if (strstr (line, "INTERFILE"))
		fprintf (fpw, "INTERFILE :=\n");
	else if (strstr (line, "number of dimensions"))
		fprintf (fpw, "number of dimensions:= %d\n", ifh->mh->ndim);
	else if (strstr (line, "matrix size [1]"))
		fprintf (fpw, "matrix size [1] := %d\n", ifh->mh->dim1);
	else if (strstr (line, "matrix size [2]"))
		fprintf (fpw, "matrix size [2] := %d\n", ifh->mh->dim2);
	else if (strstr (line, "matrix size [3]"))
		fprintf (fpw, "matrix size [3] := %d\n", ifh->mh->dim3);
	else if (strstr (line, "matrix size [4]"))
		fprintf (fpw, "matrix size [4] := %d\n", ifh->mh->dim4);
	else if (strstr (line, "scaling factor (mm/pixel) [1]"))
		fprintf (fpw, "scaling factor (mm/pixel) [1] := %f\n", ifh->mh->voxel_size_1);
	else if (strstr (line, "scaling factor (mm/pixel) [2]"))
		fprintf (fpw, "scaling factor (mm/pixel) [2] := %f\n", ifh->mh->voxel_size_2);
	else if (strstr (line, "scaling factor (mm/pixel) [3]"))
		fprintf (fpw, "scaling factor (mm/pixel) [3] := %f\n", ifh->mh->voxel_size_3);
	else if (strstr (line, "volume scale factor"))
		fprintf (fpw, "volume scale factor := %f\n", ifh->mh->scale_factor);
	else if (strstr (line, "name of data file")) {
		/* Strip off directory name. */
		filename = strrchr (ifh->mh->data_file, (char) '/');
		if (filename != NULL)
			++filename;
		else
			filename = ifh->mh->data_file;
		fprintf (fpw, "name of data file := %s\n", filename);
	} else if (strstr (line, "patient ID")) {
		if (ifh->mh->patient_id != NULL)
			fprintf (fpw, "patient ID := %s\n", ifh->mh->patient_id);
	} else if (strstr (line, "study date")) {
		if (ifh->mh->study_date != NULL)
			fprintf (fpw, "study date := %s\n", ifh->mh->study_date);
	} else if (strstr (line, "program version")) {
		if (ifh->mh->sw_revision != NULL)
			fprintf (fpw, "program version := %s\n", ifh->mh->sw_revision);
	} else if (strstr (line, "number of bed positions"))
		fprintf (fpw, "number of bed positions := %d\n", ifh->mh->nbed);
	else if (strstr (line, "bed elevation"))
		fprintf (fpw, "bed elevation := %f\n", ifh->mh->bed_elevation);
	else if (strstr (line, "initial bed position"))
		fprintf (fpw, "initial bed position := %f\n", ifh->mh->init_bed_pos);
	else if (strstr (line, "bed offset"))
		fprintf (fpw, "bed offset := %f\n", ifh->mh->bed_off[bed]);
	else if (strstr (line, "number format")) {
		if (ifh->mh->number_format == BYTE_IF) {
			fprintf (fpw, "number format := signed integer\n");
			fprintf (fpw, "number of bytes per pixel := 1\n");
		} else if (ifh->mh->number_format == SHORT_IF) {
			fprintf (fpw, "number format := signed integer\n");
			fprintf (fpw, "number of bytes per pixel := 2\n");
		} else if (ifh->mh->number_format == FLOAT_IF) {
			fprintf (fpw, "number format := float\n");
			fprintf (fpw, "number of bytes per pixel := 4\n");
		} else {
			fprintf (stderr, "ERROR [write_key]: Invalid number format\n");
			return (ERROR);
		}
	} else if (strstr (line, "number of bytes per pixel")) {
	} else if (strstr (line, "frame number")) {
	} else if (strstr (line, "frame starting time")) {
	} else if (strstr (line, "frame duration")) {
	} else if (strstr (line, "plane number")) {
	} else if (strstr (line, "storage scale factor")) {
	} else if (strstr (line, "number of trues")) {
	} else if (strstr (line, "number of randoms")) {
	} else if (strstr (line, "number of multiples")) {
	} else if (strstr (line, "deadtime correction factor")) {
	} else if (strstr (line, "user defined comment")) {
	} else {
		fprintf (stderr, "ERROR [write_key]: Could not match key string: %s\n", line);
		return (ERROR);
	}
}
