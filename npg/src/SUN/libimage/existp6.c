/*  $Id: existp6.c,v 1.3 1999/12/13 17:45:50 tom Exp $
 *  $Log: existp6.c,v $
 *  Revision 1.3  1999/12/13 17:45:50  tom
 *  tov revision
 *
 * Revision 1.2  1997/12/02  22:46:10  tom
 * modify for dynamic
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *  Function:       existp6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *  Description:    Check if an image frame exists.
 *                  Bypasses calls with error messages.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/existp6.c,v 1.3 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <image.h>

int             existp6 (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	long            offset, slices, n;

	if (file->fp == NULL) {
		file->fp = fopen (file->name, file->mode);
		if (file->fp == NULL)
			return (ERROR);
	}
	if (file->p6 == NULL)
		if ((file->p6 = (PETT6HDR *) malloc (sizeof (PETT6HDR))) == NULL)
			return (ERROR);
	slices = file->p6->number_of_slices;
	if (slices == 0)
		slices = 7;
    n = (long) ((indx->frm) - 1) * slices;
    offset = (n + (long) indx->pln - 1) * PETT6_SLICE_BYTES;
	(void) fseek (file->fp, offset, SEEK_SET);
	if ((fread (file->p6, 1, RECLEN, file->fp)) != RECLEN)
		return (ERROR);
	else
		return (OK);
}
