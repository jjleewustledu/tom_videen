/*  $Id: make_mh.c,v 1.3 2000/09/28 14:44:29 tom Exp $
    $Log: make_mh.c,v $
 * Revision 1.3  2000/09/28  14:44:29  tom
 * calloc
 *
 * Revision 1.2  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		make_mh
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Malloc and fill main header.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/make_mh.c,v 1.3 2000/09/28 14:44:29 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>

int             make_mh (file)
	ImageFile      *file;
{
	MainHeader     *mh;
	int             i;

	if ((file->mh = (MainHeader *) calloc (1, sizeof (MainHeader))) == NULL) {
		(void) fprintf (stderr, "ERROR [make_mh]: cannot calloc mainheader\n");
		return (ERROR);
	}
	mh = file->mh;
	mh->data_file = NULL;
	mh->file_format = 0;
	mh->number_format = 0;
	mh->sw_version = 0;
	mh->filetype = 0;
	mh->nplane = 0;
	mh->nframe = 0;
	mh->ngate = 0;
	mh->nbed = 0;
	mh->scanner_type = 0;
	(void) strcpy (mh->date_time, " ");
	mh->start_time = 0;
	mh->wobble_speed = -1;
	(void) strcpy (mh->patnum, " ");
	mh->bed_elevation = 0.;
	mh->init_bed_pos = 0.;
	for (i = 0; i < 16; i++)
		mh->bed_off[i] = 0.;
	mh->slicesep = 0.;
	mh->calibration_factor = 1.;
	(void) strcpy (mh->isotope, " ");
	(void) strcpy (mh->radiopharmaceutical, " ");
	mh->orientation = 0;
	mh->scale_flag = 0;
	mh->pr_scaler = 0.;
	mh->pr_filter = 0;
	mh->pett6_flag = 0;
	mh->zorigin = 0;
	mh->sw_revision = NULL;
	mh->filetime = NULL;
	mh->gen_data = NULL;
	(void) strcpy (mh->scanner, " ");
	(void) strcpy (mh->acquisition, " ");
	(void) strcpy (mh->reconstruction, " ");
	(void) strcpy (mh->filtername, " ");
	return (OK);
}
