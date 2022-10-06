/*  $Id: use_mh7.c,v 1.8 2002/03/13 21:56:41 tom Exp $
    $Log: use_mh7.c,v $
    Revision 1.8  2002/03/13 21:56:41  tom
    lint cleaning

 * Revision 1.7  2000/09/27  19:03:28  tom
 * *** empty log message ***
 *
 * Revision 1.6  1999/12/15  20:54:18  tom
 * fix localtime y2k bug
 *
 * Revision 1.5  1999/12/15  19:47:26  tom
 * year 2000
 *
 * Revision 1.4  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.3  1998/09/18  13:38:38  tom
 * add system 962
 *
 * Revision 1.2  1996/09/25  15:16:52  tom
 * add long scantime
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		use_mh7
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Fill generic mainheader structure, mh, from Version 7 mainheader
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/use_mh7.c,v 1.8 2002/03/13 21:56:41 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <image.h>
#include <libimage.h>

int             use_mh7 (file)
	ImageFile      *file;
{
	MainHeader     *mh;
	Main_header7   *mh7;
	struct tm      *ptm;
	long		    scantime;
	int             year;
	int             i;

	if (file->m7 == NULL) {
		(void) fprintf (stderr, "ERROR [use_mh7]: Version 7 header is not defined\n");
		return (ERROR);
	}
	if (file->m7->mh == NULL) {
		(void) fprintf (stderr, "ERROR [use_mh7]: Version 7 mainheader is not defined\n");
		return (ERROR);
	}
	mh7 = file->m7->mh;
	if (file->mh == NULL) {
		if (make_mh (file) == ERROR) {
			(void) fprintf (stderr, "ERROR [use_mh7]: cannot make MainHeader\n");
			return (ERROR);
		}
	}
	mh = file->mh;
	mh->file_format = MATRIX_REV7;
	mh->sw_version = (int) mh7->sw_version;
	mh->filetype = mh7->file_type;
	mh->nplane = mh7->num_planes;
	mh->nframe = mh7->num_frames;
	mh->ngate = mh7->num_gates;
	mh->nbed = mh7->num_bed_pos;
	mh->scanner_type = mh7->system_type;
    scantime = (long) mh7->scan_start_time;
	ptm = localtime (&scantime);
	year = ptm->tm_year;
	if (year < 1000)
		year += 1900;
	(void) sprintf (mh->date_time, "%02d/%02d/%04d, %02d:%02d:%02d", ptm->tm_mon + 1, ptm->tm_mday,
		 year, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	mh->start_time = mh7->scan_start_time;
	mh->wobble_speed = (int) mh7->wobble_speed;
	for (i = 0; i < 16; i++)
		mh->patnum[i] = mh7->patient_id[i];
	mh->bed_elevation = mh7->bed_elevation;
	mh->init_bed_pos = mh7->init_bed_position;
	for (i = 0; i < 16; i++)
		mh->bed_off[i] = mh7->bed_offset[i];
	mh->slicesep = mh7->plane_separation;
	mh->calibration_factor = mh7->calibration_factor;
	for (i = 0; i < 8; i++)
		mh->isotope[i] = mh7->isotope_code[i];
	for (i = 0; i < 32; i++)
		mh->radiopharmaceutical[i] = mh7->radiopharmaceutical[i];
	comment2info (mh, mh7->study_description);
	if (mh->sw_revision != NULL)
		free (mh->sw_revision);
	mh->sw_revision = (char *) calloc (1, strlen (mh7->physician_name) + 1);
	(void) strcpy (mh->sw_revision, mh7->physician_name);
	if (mh->filetime != NULL)
		free (mh->filetime);
	mh->filetime = (char *) calloc (1, strlen (mh7->operator_name) + 1);
	(void) strcpy (mh->filetime, mh7->operator_name);
	if (mh7->system_type == 953)
		(void) strcpy (mh->scanner, "953");
	else if (mh7->system_type == 961)
		(void) strcpy (mh->scanner, "961");
	else if (mh7->system_type == 962)
		(void) strcpy (mh->scanner, "962");
	else if (mh7->system_type == 6)
		(void) strcpy (mh->scanner, "pett6");
	else
		(void) strcpy (mh->scanner, "");
	if (mh7->septa_state == 0)
		(void) strcpy (mh->acquisition, "2d");
	else if (mh7->septa_state == 1)
		(void) strcpy (mh->acquisition, "3d");
	else if (mh7->septa_state == -1)	 /* for PETT VI */
		(void) strcpy (mh->acquisition, "low");
	else if (mh7->septa_state == -2)	 /* for PETT VI */
		(void) strcpy (mh->acquisition, "high");
	else
		(void) strcpy (mh->acquisition, "");
	return (OK);
}
