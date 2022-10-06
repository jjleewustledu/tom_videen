/*  $Id: use_mhif.c,v 1.5 2002/03/28 16:01:08 tom Exp $
 	$Log: use_mhif.c,v $
 	Revision 1.5  2002/03/28 16:01:08  tom
 	date_time, patnum from header

 * Revision 1.4  2002/03/13  21:56:41  tom
 * lint cleaning
 *
 * Revision 1.3  2000/09/28  14:44:29  tom
 * calloc
 *
 * Revision 1.2  2000/07/18  17:02:58  tom
 * add documentation
 *
 * Revision 1.1  2000/07/18  15:40:40  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		use_mhif
 *  Date:           Jul 2000
 *  Author:         Tom Videen
 *	Description:	Fill MainHeader structure from Interfile header
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/use_mhif.c,v 1.5 2002/03/28 16:01:08 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>

int             use_mhif (file)
	ImageFile      *file;
{
	MainHeader     *mh;
	MainHeader_if  *ifh;
	int             i;

	if (file->ih == NULL) {
		(void) fprintf (stderr, "ERROR [use_mhif]: No Interfile header defined\n");
		return (ERROR);
	}
	if (file->ih->mh == NULL) {
		(void) fprintf (stderr, "ERROR [use_mhif]: No Interfile header defined\n");
		return (ERROR);
	}
	ifh = (MainHeader_if *) file->ih->mh;
	if (file->mh == NULL) {
		if ((file->mh = (MainHeader *) calloc (1, sizeof (MainHeader))) == NULL) {
			(void) fprintf (stderr, "ERROR [use_mhif]: Could not calloc memory for MainHeader\n");
			return (ERROR);
		}
		file->mh->data_file = NULL;
		file->mh->sw_revision = NULL;
		file->mh->filetime = NULL;
		file->mh->gen_data = NULL;
	}
	mh = file->mh;
	mh->data_file = ifh->data_file;
	mh->file_format = INTERFILE;
	mh->number_format = ifh->number_format;
	mh->sw_version = ifh->sw_version;
	mh->filetype = 0;
	mh->nplane = ifh->dim3;
	mh->nframe = ifh->dim4;
	mh->ngate = 1;
	mh->nbed = 1;
	mh->scanner_type = 0;
	if (ifh->study_date != NULL)
		(void) strcpy (mh->date_time, ifh->study_date);
	else
		(void) strcpy (mh->date_time, "");
	mh->start_time = 0;
	mh->wobble_speed = 0;
	if (ifh->subject_id != NULL)
		(void) strcpy (mh->patnum, ifh->subject_id);
	else
		(void) strcpy (mh->patnum, "");
	mh->bed_elevation = 0;
	mh->init_bed_pos = 0;
	for (i = 0; i < 16; i++)
		mh->bed_off[i] = 0;
	mh->slicesep = ifh->voxel_size_3;
	mh->calibration_factor = 1.;
	(void) strcpy (mh->isotope, "");
	(void) strcpy (mh->radiopharmaceutical, "");

	if (strncmp (ifh->initial_1, "left", 4) == 0 && strncmp (ifh->initial_2, "anterior", 4) == 0 && strncmp
	    (ifh->initial_3, "superior", 4) == 0)
		mh->orientation = 0;	/*  Lab standard  */
	else if (strncmp (ifh->initial_1, "right", 4) == 0 && strncmp (ifh->initial_2, "posterior", 4) == 0 && strncmp
		 (ifh->initial_3, "inferior", 4) == 0)
		mh->orientation = 1;	/*  Analyze standard -- all dimensions flipped  */
	else if (strncmp (ifh->initial_1, "right", 4) == 0 && strncmp (ifh->initial_2, "anterior", 4) == 0 && strncmp
		 (ifh->initial_3, "inferior", 4) == 0)
		mh->orientation = 2;	/*  right/left flipped from Lab standard  */
	else
		mh->orientation = -1;

	mh->scale_flag = 0;
	mh->pr_scaler = 0;
	mh->pr_filter = 0;
	mh->pett6_flag = 0;
	mh->zorigin = 0;
	mh->sw_revision = NULL;
	mh->filetime = NULL;
	return (OK);
}
