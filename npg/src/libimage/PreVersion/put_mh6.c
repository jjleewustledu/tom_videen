/*  $Id: put_mh6.c,v 1.2 1999/12/13 17:45:50 tom Exp $
    $Log: put_mh6.c,v $
 * Revision 1.2  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		put_mh6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Write version 6 main header.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/put_mh6.c,v 1.2 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             put_mh6 (file)
	ImageFile      *file;
{
	int             status;

	if (file->m6 == NULL) {
		(void) fprintf (stderr, "ERROR [put_mh6]: No version 6 Header for %s\n", file->name);
		return (ERROR);
	}
	if (file->m6->mfp == NULL) {
		(void) fprintf (stderr, "ERROR [put_mh6]: No matrix file for %s\n", file->name);
		return (ERROR);
	}
	if (file->m6->mfp->fptr == NULL) {
		(void) fprintf (stderr, "ERROR [put_mh6]: No file pointer for %s\n", file->name);
		return (ERROR);
	}
	if (file->m6->mh == NULL) {
		(void) fprintf (stderr, "ERROR [put_mh6]: No version 6 MainHeader for %s\n", file->name);
		return (ERROR);
	}
	if ((status = mat_write_main_header (file->m6->mfp->fptr, file->m6->mh)) != 0)
		(void) fprintf (stderr, "ERROR [put_mh6]: Could not write header to %s\n", file->name);
	return (status);
}
