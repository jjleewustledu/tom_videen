/*  $Id: mhnames7.c,v 1.2 1999/12/13 17:45:50 tom Exp $
    $Log: mhnames7.c,v $
 * Revision 1.2  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		mhnames7
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Fill main header names for archive from version 7 main and subheaders.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/mhnames7.c,v 1.2 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <image.h>

#define CRIT 0.001

void            mhnames7 (file)
	ImageFile      *file;
{
	MainHeader     *mh;
	Image_subheader7 *sh7;

	mh = file->mh;
	sh7 = file->m7->sh;

	if (fabs (sh7->filter_cutoff_frequency - 1.0) <= CRIT) {
		if (strstr (mh->sw_revision, "bkproj3d") != NULL
		    && strstr (mh->sw_revision, "scatcor") != NULL
		    && strstr (mh->filetime, "makenorm") != NULL)
			(void) strcpy (mh->reconstruction, "3dbpjmon");
		else if (strstr (mh->sw_revision, "bkproj3d") != NULL
			 && strstr (mh->filetime, "makenorm") != NULL)
			(void) strcpy (mh->reconstruction, "3dbpjmos");
		else
			(void) strcpy (mh->reconstruction, "3dbp");
	} else if (fabs (sh7->filter_cutoff_frequency - 0.5) <= CRIT) {
		if (sh7->filter_scatter_fraction > 0 && sh7->filter_scatter_slope > 0)
			(void) strcpy (mh->reconstruction, "fb");
		else
			(void) strcpy (mh->reconstruction, "fbu");
	} else
		(void) strcpy (mh->reconstruction, "unknown");

	if (sh7->filter_code == 1)
		(void) strcpy (mh->filtername, "ramp");
	else if (sh7->filter_code == 2)
		(void) strcpy (mh->filtername, "butterworth");
	else if (sh7->filter_code == 3)
		(void) strcpy (mh->filtername, "hann");
	else if (sh7->filter_code == 4)
		(void) strcpy (mh->filtername, "hamming");
	else if (sh7->filter_code == 5)
		(void) strcpy (mh->filtername, "parzen");
	else if (sh7->filter_code == 6)
		(void) strcpy (mh->filtername, "shepp");
	else if (sh7->filter_code == 101)
		(void) strcpy (mh->filtername, "3d_gaussian");
	else if (sh7->filter_code == 201)
		(void) strcpy (mh->filtername, "3d_butterworth");
	else
		(void) strcpy (mh->filtername, "unknown");
}
