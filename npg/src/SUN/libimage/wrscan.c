/*  $Id: wrscan.c,v 1.1 2000/03/13 18:23:14 tom Exp $
    $Log: wrscan.c,v $
    Revision 1.1  2000/03/13 18:23:14  tom
    Initial revision
 */
/* =============================================================================
 *	Function:		wrscan
 *  Date:           Mar-2000
 *  Author:         Tom Videen
 *	Description:	Write scan data for 1 plane.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/wrscan.c,v 1.1 2000/03/13 18:23:14 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

int             wrscan (file, indx, matrix)
	ImageFile      *file;
	Index          *indx;
	short          *matrix;
{
	int             fformat;

	fformat = file->mh->file_format;

	if (fformat == MATRIX_REV7) {
		if ((wrscan7 (file, indx, matrix)) == ERROR) {
			(void) fprintf (stderr, "ERROR [wrscan]: writing %s\n", file->name);
			return (ERROR);
		}
	} else if (fformat == MATRIX_REV6) {
		if ((wrscan6 (file, indx, matrix)) == ERROR) {
			(void) fprintf (stderr, "ERROR [wrscan]: writing %s\n", file->name);
			return (ERROR);
		}
	} else {
		(void) fprintf (stderr, "ERROR [wrscan]: Unsupported file format %d in %s\n",
			 fformat, file->name);
		return (ERROR);
	}
	return (OK);
}
