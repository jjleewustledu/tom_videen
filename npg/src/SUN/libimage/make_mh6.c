/*  $Id: make_mh6.c,v 1.3 1999/12/13 17:45:50 tom Exp $
    $Log: make_mh6.c,v $
    Revision 1.3  1999/12/13 17:45:50  tom
    tov revision

 * Revision 1.2  1998/10/22  17:04:51  tom
 * fix bug
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		make_mh6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Allocate and Version 6 main header
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/make_mh6.c,v 1.3 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             make_mh6 (file)
	ImageFile      *file;
{

	if (file->m6 == NULL) {
		if ((file->m6 = (M6header *) malloc (sizeof (M6header))) == NULL) {
			(void) fprintf (stderr, "ERROR [make_mh6]: cannot malloc m6\n");
			return (ERROR);
		}
		file->m6->mfp = NULL;
		file->m6->mh = NULL;
		file->m6->sh = NULL;
	}
	if (file->m6->mh == NULL) {
		if ((file->m6->mh = (Main_header *) malloc (sizeof (Main_header))) == NULL) {
			(void) fprintf (stderr, "ERROR [make_mh6]: cannot malloc m6->mh\n");
			return (ERROR);
		}
	}
	file->m6->mh->data_type = 0;
	file->m6->mh->file_type = 0;
	return (OK);
}
