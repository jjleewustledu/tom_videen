/*  $Id: mhnames6.c,v 1.2 1999/12/13 17:45:50 tom Exp $
    $Log: mhnames6.c,v $
 * Revision 1.2  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		mhnames6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Fill main header names for archive from version 6 main and subheaders.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/mhnames6.c,v 1.2 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <image.h>

#define CRIT 0.001

void            mhnames6 (file)
	ImageFile      *file;
{
	MainHeader     *mh;
	Image_subheader *sh;

	mh = file->mh;
	sh = file->m6->sh;
	if (strncmp (mh->acquisition, "sta", 3) == 0) {
		if (fabs (sh->filter_params[0] - 1.0) <= CRIT)
			(void) strcpy (mh->acquisition, "3d");
		else if (fabs (sh->filter_params[0] - 0.5) <= CRIT)
			(void) strcpy (mh->acquisition, "sta");
		else
			(void) strcpy (mh->acquisition, "unknown");
	}
	if (fabs (sh->filter_params[0] - 1.0) <= CRIT) {
		if (strstr (mh->sw_revision, "bkproj3d") != NULL
		    && strstr (mh->sw_revision, "scatcor") != NULL
		    && strstr (mh->filetime, "makenorm") != NULL)
			(void) strcpy (mh->reconstruction, "3dbpjmon");
		else if (strstr (mh->sw_revision, "bkproj3d") != NULL
			 && strstr (mh->filetime, "makenorm") != NULL)
			(void) strcpy (mh->reconstruction, "3dbpjmos");
		else
			(void) strcpy (mh->reconstruction, "3dbp");
	} else if (fabs (sh->filter_params[0] - 0.5) <= CRIT)
		(void) strcpy (mh->reconstruction, "fb");
	else if (fabs (sh->filter_params[0] - 0.25) <= CRIT)
		(void) strcpy (mh->reconstruction, "fb");
	else
		(void) strcpy (mh->reconstruction, "unknown");

	if (abs (sh->filter_code) == 1)
		(void) strcpy (mh->filtername, "ramp");
	else if (sh->filter_code == 2)
		(void) strcpy (mh->filtername, "butterworth");
	else if (sh->filter_code == 3)
		(void) strcpy (mh->filtername, "hann");
	else if (sh->filter_code == 4)
		(void) strcpy (mh->filtername, "hamming");
	else if (sh->filter_code == 5)
		(void) strcpy (mh->filtername, "parzen");
	else if (sh->filter_code == 6)
		(void) strcpy (mh->filtername, "shepp");
	else if (abs (sh->filter_code - 100) == 1)
		(void) strcpy (mh->filtername, "3d_gaussian");
	else if (abs (sh->filter_code - 200) == 1)
		(void) strcpy (mh->filtername, "3d_butterworth");
	else
		(void) strcpy (mh->filtername, "unknown");
}
