/*  $Id: open_if.c,v 1.1 2000/07/18 15:40:33 tom Exp $ 
 *  $Log: open_if.c,v $
 *  Revision 1.1  2000/07/18 15:40:33  tom
 *  Initial revision
 * 
 */
/* =============================================================================
 *	Function:		open_if
 *  Date:           Jul-00
 *  Author:         Tom Videen
 *	Description:	Get file pointer to Interfile.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/open_if.c,v 1.1 2000/07/18 15:40:33 tom Exp $";
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

/*	Open the file */

	if ((file->fp = fopen (fname, mode)) == NULL) {
		(void) fprintf (stderr, "ERROR [open_if]: Could not open file %s\n", fname);
		return (ERROR);
	}
	return (OK);
}
