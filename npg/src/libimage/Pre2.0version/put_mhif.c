/*  $Id: put_mhif.c,v 1.7 2003/12/23 17:46:31 tom Exp $
 *  $Log: put_mhif.c,v $
 * Revision 1.7  2003/12/23  17:46:31  tom
 * check orientation
 *
 * Revision 1.6  2003/07/01  19:14:19  tom
 * *** empty log message ***
 *
 *  Revision 1.5  2002/08/20 18:26:49  tom
 *  use imgfile extension passed in
 *
 * Revision 1.4  2002/03/28  16:01:08  tom
 * changes for modality, subject id, study date
 *
 * Revision 1.3  2002/03/13  21:56:41  tom
 * lint cleaning
 *
 * Revision 1.2  2001/11/21  17:01:28  tom
 * *** empty log message ***
 *
 * Revision 1.1  2000/12/07  22:25:04  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		put_mhif
 *  Date:           Mar 2003
 *  Author:         Tom Videen
 *	Description:	Write Interfile header.
 * =============================================================================
 */
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/put_mhif.c,v 1.7 2003/12/23 17:46:31 tom Exp $";

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <image.h>
#include <interfile.h>

#define MAXSTR 256

int             put_mhif (file)
	ImageFile      *file;
{
	MainHeader_if  *ifh;
	char            version[5];
	char            program_date[12];
	char           *IF_hdr;
	char           *IF_img;
	char           *ptr;

	ptr = strstr (rcsid, "v ");
	ptr += 2;
	(void) strcpy (version, "    ");
	(void) strncpy (version, ptr, 4);
	ptr = strstr (ptr, " ");
	ptr++;
	(void) strcpy (program_date, "           ");
	(void) strncpy (program_date, ptr, 10);
	ptr = strchr (program_date, '/');
	*ptr = ':';
	ptr = strchr (program_date, '/');
	*ptr = ':';

	if (file->ih == NULL) {
		fprintf (stderr, "ERROR [put_mhif]: No Interfile header\n");
		return (ERROR);
	}
	if (file->ih->mh == NULL) {
		fprintf (stderr, "ERROR [put_mhif]: No Interfile MainHeader\n");
		return (ERROR);
	}
	ifh = file->ih->mh;

	if (file->mh->orientation == 0)
		(void) strcpy (file->ih->mh->initial_1, "left");
	else if (file->mh->orientation == 1)
		(void) strcpy (file->ih->mh->initial_1, "right");
	else if (file->mh->orientation == 2)
		(void) strcpy (file->ih->mh->initial_1, "right");

	if (file->fp != NULL)
		if (ftell (file->fp) >= 0)
			(void) fclose (file->fp);

	IF_hdr = calloc (MAXSTR, sizeof (char));
	IF_img = calloc (MAXSTR, sizeof (char));
	ptr = strrchr (file->name, '/');
	if (ptr != NULL) {
		(void) strcpy (IF_hdr, ptr + 1);
		(void) strcpy (IF_img, ptr + 1);
	} else {
		(void) strcpy (IF_hdr, file->name);
		(void) strcpy (IF_img, file->name);
	}
	ptr = strrchr (IF_hdr, '.');
	if (ptr != NULL)
		(void) strcpy (ptr, ".ifh");
	else
		(void) strcat (IF_hdr, ".ifh");
	ptr = strrchr (IF_img, '.');
	if (ptr == NULL)
		(void) strcat (IF_hdr, ".img");

	file->fp = fopen (IF_hdr, file->mode);
	if (file->fp == NULL) {
		(void) fprintf (stderr, "ERROR [put_mhif]: Could not open Interfile header %s\n", IF_hdr);
		return (ERROR);
	}
	fprintf (file->fp, "INTERFILE :=\n");
	if (ifh->modality != NULL)
		fprintf (file->fp, "image modality     := %s\n", ifh->modality);
	fprintf (file->fp, "originating system := ECAT/%d\n", file->mh->scanner_type);
	fprintf (file->fp, "version of keys    := 3.3\n");
	fprintf (file->fp, "date of keys       := 1992:01:01\n");
	fprintf (file->fp, "conversion program := put_mhif\n");
	fprintf (file->fp, "program version    := %s\n", version);
	fprintf (file->fp, "program date       := %s\n", program_date);
	fprintf (file->fp, ";\n");
	fprintf (file->fp, "GENERAL DATA :=\n");
	fprintf (file->fp, "original institution := %s\n", ifh->institution);
	fprintf (file->fp, "data starting block  := 0\n");
	fprintf (file->fp, "name of data file    := %s\n", IF_img);
	if (ifh->subject_id != NULL)
		fprintf (file->fp, "patient ID := %s\n", ifh->subject_id);
	else if (file->mh->patnum != NULL)
		fprintf (file->fp, "patient ID := %s\n", file->mh->patnum);
	if (ifh->study_date != NULL)
		fprintf (file->fp, "study date := %s\n", ifh->study_date);
	else if (file->mh->date_time != NULL)
		fprintf (file->fp, "study date := %s\n", file->mh->date_time);
	fprintf (file->fp, ";\n");
	fprintf (file->fp, "GENERAL IMAGE DATA :=\n");
	fprintf (file->fp, "type of data := tomographic\n");
	if (ifh->ndim == 4)
		fprintf (file->fp, "total number of images := %d\n", ifh->dim3 * ifh->dim4);
	else
		fprintf (file->fp, "total number of images := %d\n", ifh->dim3);
	if (ifh->big_endian == 0)
		fprintf (file->fp, "imagedata byte order   := littleendian\n");
	else
		fprintf (file->fp, "imagedata byte order   := bigendian\n");
	fprintf (file->fp, ";\n");
	fprintf (file->fp, "PET STUDY :=\n");
	fprintf (file->fp, "process status := reconstructed\n");
	fprintf (file->fp, "number of dimensions := %d\n", ifh->ndim);
	fprintf (file->fp, "number of slices := %d\n", ifh->dim3);
	fprintf (file->fp, "matrix size [1] := %d\n", ifh->dim1);
	fprintf (file->fp, "matrix size [2] := %d\n", ifh->dim2);
	fprintf (file->fp, "matrix size [3] := %d\n", ifh->dim3);
	fprintf (file->fp, "matrix size [4] := %d\n", ifh->dim4);
	if (ifh->number_format == BYTE_DATA)
		fprintf (file->fp, "number format   := byte\n");
	else if (ifh->number_format == SHORT_DATA)
		fprintf (file->fp, "number format   := signed integer\n");
	else if (ifh->number_format == FLOAT_DATA)
		fprintf (file->fp, "number format   := float\n");
	else if (ifh->number_format == INT_DATA)
		fprintf (file->fp, "number format   := signed integer\n");
	else if (file->mh->number_format == SHORT_DATA)
		fprintf (file->fp, "number format   := signed integer\n");
	else if (file->mh->number_format == INT_DATA)
		fprintf (file->fp, "number format   := signed integer\n");
	else if (file->mh->number_format == FLOAT_DATA)
		fprintf (file->fp, "number format   := float\n");
	fprintf (file->fp, "number of bytes per pixel  := %d\n", ifh->nbytes);
	fprintf (file->fp, "scaling factor (mm/pixel) [1]  := %f\n", ifh->voxel_size_1);
	fprintf (file->fp, "scaling factor (mm/pixel) [2]  := %f\n", ifh->voxel_size_2);
	fprintf (file->fp, "scaling factor (mm/pixel) [3]  := %f\n", ifh->voxel_size_3);
	fprintf (file->fp, "matrix initial element [1] := %s\n", ifh->initial_1);
	fprintf (file->fp, "matrix initial element [2] := %s\n", ifh->initial_2);
	fprintf (file->fp, "matrix initial element [3] := %s\n", ifh->initial_3);
	if (file->mh->zorigin > 0) {
		fprintf (file->fp, "atlas origin [1] := %.2f\n", 0.5 * ((float) (ifh->dim1 + 1)));
		fprintf (file->fp, "atlas origin [2] := %.2f\n", 0.5 * ((float) (ifh->dim2 + 1)));
		fprintf (file->fp, "atlas origin [3] := %d\n", file->mh->zorigin);
	}
/*
	fprintf (file->fp, "maximum pixel count := %d\n", global_max);
	fprintf (file->fp, "minimum pixel count := %d\n", global_min);
*/
	fprintf (file->fp, "END OF INTERFILE :=\n");
	(void) fclose (file->fp);
	return (OK);
}
