/*  $Id: make_sh6.c,v 1.2 1999/12/13 17:45:50 tom Exp $
    $Log: make_sh6.c,v $
 * Revision 1.2  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		make_sh6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Allocate a Version 6 image subheader.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/make_sh6.c,v 1.2 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             make_sh6 (file)
	ImageFile      *file;
{

	if (file->m6->sh == NULL) {
		if ((file->m6->sh = (Image_subheader *) malloc (sizeof (Image_subheader))) == NULL) {
			(void) fprintf (stderr, "ERROR [make_sh6]: cannot malloc m6->sh\n");
			return (ERROR);
		}
	}
	file->m6->sh->data_type = 0;
	return (OK);
}
