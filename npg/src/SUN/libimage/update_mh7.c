/*  $Id: update_mh7.c,v 1.2 1999/12/13 17:45:50 tom Exp $
    $Log: update_mh7.c,v $
    Revision 1.2  1999/12/13 17:45:50  tom
    tov revision

 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		update_mh7
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Update select Version 7 mainheader fields using mainheader structure
 * =============================================================================
 */

#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/update_mh7.c,v 1.2 1999/12/13 17:45:50 tom Exp $";

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>
#include <libimage.h>

int             update_mh7 (file)
	ImageFile      *file;
{
	MainHeader     *mh;
	Main_header7   *mh7;
	int             i;

	if (file->mh == NULL) {
		(void) fprintf (stderr, "ERROR [update_mh7]: Mainheader is not defined\n");
		return (ERROR);
	}
	if (file->m7 == NULL) {
		(void) fprintf (stderr, "ERROR [update_mh7]: Version 7 header is not defined\n");
		return (ERROR);
	}
	if (file->m7->mh == NULL) {
		(void) fprintf (stderr, "ERROR [update_mh7]: Version 7 header is not defined\n");
		return (ERROR);
	}
	mh = file->mh;
	mh7 = file->m7->mh;

	for (i = 0; i < 8; i++)
		mh7->isotope_code[i] = mh->isotope[i];
	for (i = 0; i < 32; i++)
		mh7->radiopharmaceutical[i] = mh->radiopharmaceutical[i];
	for (i = 0; i < 16; i++)
		mh7->patient_id[i] = mh->patnum[i];
	(void) strncpy (mh7->physician_name, mh->sw_revision, 31);
	(void) strncpy (mh7->operator_name, mh->filetime, 31);
	info2comment (mh, mh7->study_description);
	mh7->num_planes = (short) mh->nplane;
	mh7->num_frames = (short) mh->nframe;
	mh7->plane_separation = mh->slicesep;
	return (OK);
}
