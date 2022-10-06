/*  $Id: fill_mh7.c,v 1.4 2002/03/13 21:56:41 tom Exp $
    $Log: fill_mh7.c,v $
 * Revision 1.4  2002/03/13  21:56:41  tom
 * lint cleaning
 *
 * Revision 1.3  2000/09/27  19:03:28  tom
 * *** empty log message ***
 *
 * Revision 1.2  1996/06/04  16:47:55  tom
 * make sure filetype and calibration_factor are defined
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		fill_mh7
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Fill from Version 7 mainheader from generic mainheader structure, mh
 * =============================================================================
 */

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/fill_mh7.c,v 1.4 2002/03/13 21:56:41 tom Exp $";

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>
#include <libimage.h>

int             fill_mh7 (file)
	ImageFile      *file;
{
	MainHeader     *mh;
	Main_header7   *mh7;
	int             i;

	if (file->mh == NULL) {
		fprintf (stderr, "ERROR [fill_mh7]: Mainheader is not defined\n");
		return (ERROR);
	}
	if (file->m7 == NULL) {
		fprintf (stderr, "ERROR [fill_mh7]: Version 7 header is not defined\n");
		return (ERROR);
	}
	if (file->m7->mh == NULL) {
		if (make_mh7 (file) == ERROR) {
			fprintf (stderr, "ERROR [make_mh7]: cannot calloc m7->mh\n");
			return (ERROR);
		}
	}
	mh = file->mh;
	mh7 = file->m7->mh;

	(void) strcpy (mh7->magic_number, "MATRIX7");

	mh7->sw_version = (short) mh->sw_version;
	mh7->system_type = (short) mh->scanner_type;
	if (mh->filetype > 0)
		mh7->file_type = (short) mh->filetype;
	else
		mh7->file_type = PetVolume;

	mh7->scan_start_time = mh->start_time;
	for (i = 0; i < 8; i++)
		mh7->isotope_code[i] = mh->isotope[i];
	for (i = 0; i < 32; i++)
		mh7->radiopharmaceutical[i] = mh->radiopharmaceutical[i];

	mh7->bed_elevation = mh->bed_elevation;

	mh7->wobble_speed = (short) mh->wobble_speed;

	if (mh->calibration_factor > 0.)
		mh7->calibration_factor = mh->calibration_factor;
	else
		mh7->calibration_factor = 1.;

	for (i = 0; i < 16; i++)
		mh7->patient_id[i] = mh->patnum[i];

	(void) strncpy (mh7->physician_name, mh->sw_revision, 31);
	(void) strncpy (mh7->operator_name, mh->filetime, 31);
	info2comment (mh, mh7->study_description);

	mh7->num_planes = (short) mh->nplane;
	mh7->num_frames = (short) mh->nframe;
	mh7->num_gates = (short) mh->ngate;
	mh7->num_bed_pos = (short) mh->nbed;

	mh7->init_bed_position = mh->init_bed_pos;
	for (i = 0; i < 16; i++)
		mh7->bed_offset[i] = mh->bed_off[i];
	mh7->plane_separation = mh->slicesep;

	if (strcmp (mh->acquisition, "2d") == 0)
		mh7->septa_state = 0;
	else if (strcmp (mh->acquisition, "3d") == 0)
		mh7->septa_state = 1;
	else
		mh7->septa_state = 0;
	return (OK);
}
