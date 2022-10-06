/*  $Id: open_p6.c,v 1.3 2000/07/12 19:53:03 tom Exp $
 *  $Log: open_p6.c,v $
 * Revision 1.3  2000/07/12  19:53:03  tom
 * *** empty log message ***
 *
 * Revision 1.2  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		open_p6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Get file pointer to PETT VI file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/open_p6.c,v 1.3 2000/07/12 19:53:03 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>

int             open_p6 (file)
	ImageFile      *file;
{
	FILE           *fp;
	char           *fname;
	char           *mode;
	char            cmd[MAXPATH];

	fname = file->name;
	mode = file->mode;

/* Delete the file if mode is write. */

	if (strcmp (mode, "w") == 0) {
		if (file->fp != NULL)
			(void) fclose (file->fp);
		if ((fp = fopen (file->name, "r")) != NULL) {
			(void) fclose (fp);
			(void) sprintf (cmd, "rm %s\n", fname);
			(void) system (cmd);
			(void) fprintf (stderr, "Warning: Deleted %s\n", fname);
		}
	}
	if ((file->fp = fopen (fname, mode)) == NULL) {
		(void) fprintf (stderr, "ERROR [open_p6]: Could not open file %s\n", fname);
		return (ERROR);
	}
	return (OK);
}
