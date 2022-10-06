/*  $Id: get_hp6.c,v 1.4 1999/12/13 17:45:50 tom Exp $
 *  $Log: get_hp6.c,v $
 * Revision 1.4  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.3  1997/12/02  22:55:59  tom
 * add unistd.h
 *
 * Revision 1.2  1997/12/02  22:46:21  tom
 * modify for dynamic
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		get_hp6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Read PETT VI slice header and store in PETT6HDR.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/get_hp6.c,v 1.4 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <image.h>

int             get_hp6 (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	long            offset, slices, n;

	if (file->p6 == NULL) {
		if ((file->p6 = (PETT6HDR *) malloc (sizeof (PETT6HDR))) == NULL) {
			(void) fprintf (stderr, "ERROR [get_hp6]: Could not malloc memory for PETT VI header\n");
			return (ERROR);
		}
	}
    slices = file->p6->number_of_slices;
    if (slices == 0)
        slices = 7;
    n = (long) ((indx->frm) - 1) * slices;
    offset = (n + (long) indx->pln - 1) * PETT6_SLICE_BYTES;
	(void) fseek (file->fp, offset, SEEK_SET);
	if ((fread (file->p6, 1, RECLEN, file->fp)) != RECLEN) {
		(void) fprintf (stderr, "ERROR [get_hp6]: reading PETT VI header for %s\n", file->name);
		return (ERROR);
	}
	return (OK);
}
