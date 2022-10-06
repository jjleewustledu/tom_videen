/*  $Id: update_sh.c,v 1.2 1999/12/13 17:45:50 tom Exp $
    $Log: update_sh.c,v $
 * Revision 1.2  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		update_sh
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Update select fields of image subheader.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/update_sh.c,v 1.2 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <image.h>
#include <libimage.h>

int             update_sh (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	int             fformat;

	fformat = file->mh->file_format;

	if (fformat == MATRIX_REV7) {
		if ((update_sh7 (file)) == ERROR) 
			return (ERROR);
	} else if (fformat == MATRIX_REV6) {
		if ((update_sh6 (file)) == ERROR) 
			return (ERROR);
	} else if (fformat == PETT6) {
		if ((update_hp6 (file, indx)) == ERROR) 
			return (ERROR);
#ifdef UNDEF
	} else if (fformat == INTERFILE) {
		if ((update_shif (file, indx)) == ERROR)
			return (ERROR);
#endif
	} else {
		(void) fprintf (stderr, "ERROR [update_sh]: Unsupported file format %d in %s\n",
			 fformat, file->name);
		return (ERROR);
	}
	return (OK);
}
