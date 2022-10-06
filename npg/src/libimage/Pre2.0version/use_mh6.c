/*  $Id: use_mh6.c,v 1.9 2002/03/13 21:56:41 tom Exp $
    $Log: use_mh6.c,v $
 * Revision 1.9  2002/03/13  21:56:41  tom
 * lint cleaning
 *
 * Revision 1.8  1999/12/15  20:54:18  tom
 * fix localtime y2k bug
 *
 * Revision 1.7  1999/12/15  19:47:26  tom
 * year 2000
 *
 * Revision 1.6  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.5  1999/04/09  20:12:52  tom
 * removed f3d check as this does not appear necessary
 *
 * Revision 1.4  1999/04/09  19:03:56  tom
 * *** empty log message ***
 *
 * Revision 1.3  1999/04/09  19:00:04  tom
 * if physician_name contains f3d, then this is a 3D acquisition
 *
 * Revision 1.2  1998/09/18  13:38:19  tom
 * add system 962
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		use_mh6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Fill generic mainheader structure, mh, from Version 6 mainheader
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/use_mh6.c,v 1.9 2002/03/13 21:56:41 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>
#include <libimage.h>

#define CRIT 0.001

int             use_mh6 (file)
	ImageFile      *file;
{
	MainHeader     *mh;
	Main_header    *mh6;
	int             year;
	int             i;

	if (file->m6 == NULL) {
		(void) fprintf (stderr, "ERROR [use_mh6]: Version 6 mainheader is not defined\n");
		return (ERROR);
	}
	if (file->m6->mh == NULL) {
		(void) fprintf (stderr, "ERROR [use_mh6]: Version 6 mainheader is not defined\n");
		return (ERROR);
	}
	mh6 = file->m6->mh;
	if (file->mh == NULL) {
		if (make_mh (file) == ERROR) {
			(void) fprintf (stderr, "ERROR [use_mh6]: cannot make MainHeader\n");
			return (ERROR);
		}
	}
	mh = file->mh;
	mh->file_format = MATRIX_REV6;
	if (mh6->data_type == VAX_I2 || mh6->data_type == SUN_I2)
		mh->number_format = SHORT_DATA;
	else if (mh6->data_type == BYTE_TYPE)
		mh->number_format = BYTE_DATA;
	else if (mh6->data_type == IEEE_R4)
		mh->number_format = FLOAT_DATA;
	mh->sw_version = (int) mh6->sw_version;
	mh->filetype = mh6->file_type;
	mh->nplane = mh6->num_planes;
	mh->nframe = mh6->num_frames;
	mh->ngate = mh6->num_gates;
	mh->nbed = mh6->num_bed_pos;
	mh->scanner_type = mh6->system_type;
	year = mh6->scan_start_year;
	if (year < 100)
		year += 1900;
	(void) sprintf (mh->date_time, "%02d/%02d/%04d, %02d:%02d:%02d",
		 mh6->scan_start_month, mh6->scan_start_day, year,
		 mh6->scan_start_hour, mh6->scan_start_minute, mh6->scan_start_second);
	mh->start_time = -1;
	mh->wobble_speed = (int) mh6->wobble_speed;
	for (i = 0; i < 16; i++)
		mh->patnum[i] = mh6->patient_id[i];
	mh->bed_elevation = mh6->bed_elevation;
	mh->init_bed_pos = mh6->init_bed_position;
	for (i = 0; i < 15; i++)
		mh->bed_off[i] = mh6->bed_offset[i];
	mh->slicesep = mh6->plane_separation;
	mh->calibration_factor = 1.;
	for (i = 0; i < 8; i++)
		mh->isotope[i] = mh6->isotope_code[i];
	for (i = 0; i < 32; i++)
		mh->radiopharmaceutical[i] = mh6->radiopharmaceutical[i];
	comment2info (mh, mh6->study_description);
	if (mh->sw_revision != NULL)
		free (mh->sw_revision);
	mh->sw_revision = (char *) malloc (strlen (mh6->physician_name) + 1);
	(void) strcpy (mh->sw_revision, mh6->physician_name);
	if (mh->filetime != NULL)
		free (mh->filetime);
	mh->filetime = (char *) malloc (strlen (mh6->operator_name) + 1);
	(void) strcpy (mh->filetime, mh6->operator_name);
	if (mh6->system_type == 953)
		(void) strcpy (mh->scanner, "953");
	else if (mh6->system_type == 961)
		(void) strcpy (mh->scanner, "961");
	else if (mh6->system_type == 962)
		(void) strcpy (mh->scanner, "962");
	else if (mh6->system_type == 6)
		(void) strcpy (mh->scanner, "pett6");
	else
		(void) strcpy (mh->scanner, "");
	if (mh6->wobble_speed == 0)
		(void) strcpy (mh->acquisition, "sta");
	else
		(void) strcpy (mh->acquisition, "wob");

/*	if (strstr (mh6->physician_name,"f3d") != NULL)
		(void) strcpy (mh->acquisition, "3d");
 */
	return (OK);
}
