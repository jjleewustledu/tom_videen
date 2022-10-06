/*  $Id: update_mh.c,v 1.2 1999/12/13 17:45:50 tom Exp $
    $Log: update_mh.c,v $
 * Revision 1.2  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		update_mh
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Update select fiels of image MainHeader.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/update_mh.c,v 1.2 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

int             update_mh (file)
	ImageFile      *file;
{
	Index			indx;
	int             fformat;

	fformat = file->mh->file_format;
	indx.frm = 1;
	indx.pln = 1;
	indx.gate = 0;
	indx.data = 0;
	indx.bed = 0;

	if (fformat == MATRIX_REV7) {
		if ((update_mh7 (file)) == ERROR)
			return (ERROR);
	} else if (fformat == MATRIX_REV6) {
		if ((update_mh6 (file)) == ERROR) 
			return (ERROR);
	} else if (fformat == PETT6) {
		if ((update_hp6 (file, &indx)) == ERROR) 
			return (ERROR);
#ifdef UNDEF
	} else if (fformat == INTERFILE) {
		if ((update_mhif (file)) == ERROR)
			return (ERROR);
#endif
	} else {
		(void) fprintf (stderr, "ERROR [update_mh]: Unsupported file format %d for %s\n", fformat, file->name);
		return (ERROR);
	}
	return (OK);
}
