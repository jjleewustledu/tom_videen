/*  $Id: update_hp6.c,v 1.2 1999/12/13 17:45:50 tom Exp $
 *  $Log: update_hp6.c,v $
 * Revision 1.2  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		update_hp6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Update select field of PETT VI header from MainHeader structure
 * =============================================================================
 */

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/update_hp6.c,v 1.2 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             update_hp6 (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	PETT6HDR       *p6h;
	int             i;

	if (file->mh == NULL) {
		(void) fprintf (stderr, "ERROR [update_hp6]: Mainheader is not defined\n");
		return (ERROR);
	}
	if (file->p6 == NULL) {
		(void) fprintf (stderr, "ERROR [update_hp6]: PETT VI header is not defined\n");
		return (ERROR);
	}
	p6h = file->p6;

	for (i = 0; i < 6; i++)
		p6h->run_name[i] = file->mh->radiopharmaceutical[i];
	p6h->scan_time = (short) (file->sh->duration / 1000);
	for (i = 0; i < 8; i++)
		p6h->subject_id[i] = file->mh->patnum[i];
	p6h->slice_number = indx->pln;
	p6h->number_of_slices = file->mh->nplane;
	return (OK);
}
