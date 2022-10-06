/*  $Id: open_m6.c,v 1.4 2000/07/12 19:50:16 tom Exp tom $
    $Log: open_m6.c,v $
    Revision 1.4  2000/07/12 19:50:16  tom
    *** empty log message ***

 * Revision 1.3  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.2  1999/05/14  17:58:14  tom
 * eliminate warning
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		open_m6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Get file pointer to Matrix 6 file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/RCS/open_m6.c,v 1.4 2000/07/12 19:50:16 tom Exp tom $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>

int             open_m6 (file)
	ImageFile      *file;
{
	FILE           *fp;
	char            cmd[MAXPATH];
	char           *fname;
	char           *mode;

	fname = file->name;
	mode = file->mode;

/* Delete the file if mode is write. */

	if (file->m6 == NULL) {
		if ((file->m6 = (M6header *) malloc (sizeof (M6header))) == NULL) {
			(void) fprintf (stderr, "ERROR [open_m6]: malloc m6 error for %s\n", fname);
			return (ERROR);
		}
		file->m6->mfp = NULL;
		file->m6->mh = NULL;
		file->m6->sh = NULL;
	}
	if (strcmp (mode, "w") == 0) {
		if (file->m6->mfp != NULL) {
			matrix_close (file->m6->mfp);
		}
		if ((fp = fopen (file->name, "r")) != NULL) {
			(void) fclose (fp);
			(void) sprintf (cmd, "rm %s\n", fname);
			(void) system (cmd);
		}
	}
	if (file->m6->mh == NULL) {
		if ((file->m6->mh = (Main_header *) malloc (sizeof (Main_header))) == NULL) {
			(void) fprintf (stderr, "ERROR [open_m6]: malloc m6->mh error for %s\n", fname);
			return (ERROR);
		}
	}
	if (strcmp (mode, "w") == 0) {
		if ((file->m6->mfp = matrix_create (fname, file->m6->mh)) == NULL) {
			(void) fprintf (stderr, "ERROR [open_m6]: matrix_create error for %s\n", fname);
			return (ERROR);
		}
	} else {
		if ((file->m6->mfp = matrix_open (fname, mode)) == NULL) {
			(void) fprintf (stderr, "ERROR [open_m6]: matrix_open error for %s\n", fname);
			return (ERROR);
		}
	}
	return (OK);
}
