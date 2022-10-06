/*  $Id: put_mh.c,v 1.4 2002/12/17 21:55:15 tom Exp $
    $Log: put_mh.c,v $
    Revision 1.4  2002/12/17 21:55:15  tom
    fix pointer/cast errors found with gcc

    Revision 1.3  2001/11/21 17:01:28  tom
    add put_mhif

 * Revision 1.2  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		put_mh
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Update image mainheader from MainHeader, then write image mainheader.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/put_mh.c,v 1.4 2002/12/17 21:55:15 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

int             put_mh (file)
	ImageFile      *file;
{
	int             fformat;

	fformat = file->mh->file_format;

	if (fformat == MATRIX_REV7) {
		if (update_mh7 (file) == ERROR) {
			(void) fprintf (stderr, "ERROR [put_mh]: Error updating mainheader for %s\n", file->name);
			return (ERROR);
		}
		if ((put_mh7 (file)) == ERROR) {
			(void) fprintf (stderr, "ERROR [put_mh]: Error writing main header for %s\n", file->name);
			return (ERROR);
		}
	} else if (fformat == MATRIX_REV6) {
		if (update_mh6 (file) == ERROR) {
			(void) fprintf (stderr, "ERROR [put_mh]: Error updating mainheader for %s\n", file->name);
			return (ERROR);
		}
		if ((put_mh6 (file)) == ERROR) {
			(void) fprintf (stderr, "ERROR [put_mh]: Error writing main header for %s\n", file->name);
			return (ERROR);
		}
	} else if (fformat == INTERFILE) {
		if ((put_mhif (file)) != ERROR) {
			(void) fprintf (stderr, "ERROR [put_mh]: Error writing main header for %s\n", file->name);
			return (ERROR);
		}
/* PETT VI images have no separate main header */
	} else if (fformat != PETT6) {
		(void) fprintf (stderr, "ERROR [put_mh]: Unsupported file format %d for %s\n", fformat, file->name);
		return (ERROR);
	}
	return (OK);
}
