/*  $Id: open_if.c,v 1.2 2003/07/01 19:14:19 tom Exp $
 *  $Log: open_if.c,v $
 * Revision 1.2  2003/07/01  19:14:19  tom
 * *** empty log message ***
 *
 *  Revision 1.1  2000/07/18 15:40:33  tom
 *  Initial revision
 *
 */
/* =============================================================================
 *	Function:		open_if
 *  Date:           Mar 2003
 *  Author:         Tom Videen
 *	Description:	Get file pointer to Interfile.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/open_if.c,v 1.2 2003/07/01 19:14:19 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>

int             open_if (file)
	ImageFile      *file;
{
	FILE           *fp;
	char           *fname;
	char           *mode;
	char            cmd[MAXPATH];

	fp = file->fp;
	fname = file->name;
	mode = file->mode;

/* Delete the file if mode is write. */

	if (strcmp (mode, "w") == 0) {
		if (fp != NULL)
			(void) fclose (fp);
		if ((fp = fopen (fname, "r")) != NULL) {
			(void) fclose (fp);
			(void) sprintf (cmd, "rm %s\n", fname);
			(void) system (cmd);
			(void) fprintf (stderr, "Warning: Deleted %s\n", fname);
		}
	}
/*	Open the file */

	if ((fp = fopen (fname, mode)) == NULL) {
		(void) fprintf (stderr, "ERROR [open_if]: opening %s\n", fname);
		return (ERROR);
	}
	file->fp = fp;
	return (OK);
}
