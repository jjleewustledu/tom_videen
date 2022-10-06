/*  $Id: open_imgfile.c,v 1.4 2002/03/13 21:56:41 tom Exp $
 *  $Log: open_imgfile.c,v $
 * Revision 1.4  2002/03/13  21:56:41  tom
 * lint cleaning
 *
 * Revision 1.3  2000/07/18  15:40:34  tom
 * add Interfile compatibility
 *
 * Revision 1.2  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		open_imgfile
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Get file pointer to ImageFile.
 *	Notes: The following must be defined:
 *		file->name
 *		file->mode
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/open_imgfile.c,v 1.4 2002/03/13 21:56:41 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>
#include <libimage.h>

int             open_imgfile (file)
	ImageFile      *file;
{
	int             fformat;

	if (file->name == NULL) {
		(void) fprintf (stderr, "ERROR [open_imgfile]: filename is NULL\n");
		return (ERROR);
	}
	if (strcmp (file->mode, "w") == 0)
		fformat = file->mh->file_format;
	else
		fformat = getfiletype (file);

	if (fformat == MATRIX_REV7) {
		if (open_m7 (file) == ERROR) {
			(void) fprintf (stderr, "ERROR [open_imgfile]: Cannot open %s\n", file->name);
			return (ERROR);
		}
	} else if (fformat == MATRIX_REV6) {
		if (open_m6 (file) == ERROR) {
			(void) fprintf (stderr, "ERROR [open_imgfile]: Cannot open %s\n", file->name);
			return (ERROR);
		}
	} else if (fformat == PETT6) {
		if (open_p6 (file) == ERROR) {
			(void) fprintf (stderr, "ERROR [open_imgfile]: Cannot open %s\n", file->name);
			return (ERROR);
		}
	} else if (fformat == INTERFILE) {
		if (open_if (file) == ERROR) {
			(void) fprintf (stderr, "ERROR [open_imgfile]: Cannot open %s\n", file->name);
			return (ERROR);
		}
	} else {
		(void) fprintf (stderr, "ERROR [open_imgfile]: Cannot open %s\n", file->name);
		return (ERROR);
	}
	return (OK);
}
