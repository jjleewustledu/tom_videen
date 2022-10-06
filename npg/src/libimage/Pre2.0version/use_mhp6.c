/*  $Id: use_mhp6.c,v 1.7 2003/07/01 19:14:19 tom Exp $
 *  $Log: use_mhp6.c,v $
 * Revision 1.7  2003/07/01  19:14:19  tom
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
 * Revision 1.3  1997/09/12  21:42:32  tom
 * check for null filtername
 *
 * Revision 1.2  1996/10/23  16:17:11  tom
 * add null to strncpy
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		use_mhp6
 *  Date:           Mar 2003
 *  Author:         Tom Videen
 *	Description:	Fill MainHeader structure from PETT VI header.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/use_mhp6.c,v 1.7 2003/07/01 19:14:19 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>

int             use_mhp6 (file)
	ImageFile      *file;
{
	MainHeader     *mh;
	PETT6HDR       *p6;
	float           pe2ieee ();
	char            month[3];
	char            day[3];
	char            year[3];
	char           *ptr;
	float           decay_constant;
	int             len, i;

	if (file->p6 == NULL) {
		(void) fprintf (stderr, "ERROR [use_mhp6]: No PETT6HDR defined\n");
		return (ERROR);
	}
	p6 = file->p6;
	if (file->mh == NULL) {
		if ((file->mh = (MainHeader *) malloc (sizeof (MainHeader))) == NULL) {
			(void) fprintf (stderr, "ERROR [use_mhp6]: Could not malloc memory for MainHeader\n");
			return (ERROR);
		}
		file->mh->data_file = NULL;
		file->mh->sw_revision = NULL;
		file->mh->filetime = NULL;
		file->mh->gen_data = NULL;
	}
	mh = file->mh;
	mh->file_format = PETT6;
	mh->number_format = SHORT_DATA;
	if (p6->file_type == 1000) {
		mh->sw_version = VERSION_OLD;
	} else if (p6->file_type == 1010 || p6->file_type == 1020) {
		mh->sw_version = VERSION_NEW;
	} else if (p6->file_type == 2000) {
		mh->sw_version = VERSION_ML;
	} else if (p6->file_type == 2010) {
		mh->sw_version = VERSION_DGP;
	} else if (p6->file_type == 3000) {
		mh->sw_version = VERSION_ECAT;
	}
	mh->filetype = (int) PetImage;
	if (p6->number_of_slices > 0)
		mh->nplane = p6->number_of_slices;
	else
		mh->nplane = 7;
	mh->nframe = 1;
	mh->ngate = 0;
	mh->nbed = 0;
	mh->scanner_type = 6;
	month[0] = p6->date[0];
	month[1] = p6->date[1];
	month[2] = '\0';
	day[0] = p6->date[2];
	day[1] = p6->date[3];
	day[2] = '\0';
	year[0] = p6->date[4];
	year[1] = p6->date[5];
	year[2] = '\0';
	(void) sprintf (mh->date_time, "%02d/%02d/%04d, %02d:%02d:%02d",
			atoi (month), atoi (day), 1900 + atoi (year), 0, 0, 0);
	mh->start_time = 0;
	mh->wobble_speed = 60;
	(void) strncpy (mh->patnum, p6->subject_id, 8);
	mh->bed_elevation = 0;
	mh->init_bed_pos = 0;
	for (i = 0; i < 16; i++)
		mh->bed_off[i] = 0;
	if (mh->nplane >= 49)			 /* Slice thickness not stored for PETT VI */
		mh->slicesep = 0.2;
	else
		mh->slicesep = PETT6_SLICE_SEP;
	mh->calibration_factor = 1.;
	decay_constant = pe2ieee ((unsigned int) p6->decay_constant);
	if (decay_constant >= 5.5e-3 && decay_constant <= 5.7e-3) {
		(void) strcpy (mh->isotope, "O-15");
	} else if (decay_constant >= 1.1e-3 && decay_constant <= 1.2e-3) {
		(void) strcpy (mh->isotope, "N-13");
	} else if (decay_constant >= 5.5e-4 && decay_constant <= 5.7e-4) {
		(void) strcpy (mh->isotope, "C-11");
	} else if (decay_constant >= 1.6e-4 && decay_constant <= 1.7e-4) {
		(void) strcpy (mh->isotope, "Ga-68");
	} else if (decay_constant >= 1.0e-4 && decay_constant <= 1.1e-4) {
		(void) strcpy (mh->isotope, "F-18");
	} else {
		(void) strcpy (mh->isotope, "unknown");
	}
	(void) strcpy (mh->radiopharmaceutical, p6->run_name);
	mh->orientation = 0;
	mh->scale_flag = 0;
	mh->pr_scaler = 0.0;
	if (strncmp (p6->filter, "FILTERG", 7) == 0)
		mh->pr_filter = FILTERG;
	else if (strncmp (p6->filter, "FILTERF", 7) == 0)
		mh->pr_filter = FILTERF;
	else
		mh->pr_filter = 0;
	mh->pett6_flag = 1;
	if (p6->number_of_slices < 31)
		mh->zorigin = 0;
	else
		mh->zorigin = 31;
	(void) strcpy (mh->scanner, "pett6");
	if (p6->resolution == 0)
		(void) strcpy (mh->acquisition, "low");
	else if (p6->resolution == 1)
		(void) strcpy (mh->acquisition, "high");
	else
		(void) strcpy (mh->acquisition, "unknown");

	if (p6->file_type == 1000)
		(void) strcpy (mh->reconstruction, "fbold");
	else if (p6->file_type == 1010 || p6->file_type == 1020)
		(void) strcpy (mh->reconstruction, "fb");
	else if (p6->file_type == 2000)
		(void) strcpy (mh->reconstruction, "ml");
	else if (p6->file_type == 2010)
		(void) strcpy (mh->reconstruction, "fbdgp");
	else
		(void) strcpy (mh->reconstruction, "unknown");

	if (strncmp (p6->filter, "FILTERG", 7) == 0)
		(void) strcpy (mh->filtername, "filterg");
	else if (strncmp (p6->filter, "FILTERF", 7) == 0)
		(void) strcpy (mh->filtername, "filterf");
	else if (p6->file_type == 1000) {
		if (p6->resolution == 0)
			(void) strcpy (mh->filtername, "filterg");
		else if (p6->resolution == 1)
			(void) strcpy (mh->filtername, "filterf");
	} else if (strlen (p6->filter) > 1) {
		ptr = strchr (p6->filter, ' ');
		len = ptr - p6->filter;
		if (len > 10)
			len = 10;
		(void) strncpy (mh->filtername, p6->filter, len);
		*(mh->filtername + len) = '\0';
	}
	return (OK);
}
