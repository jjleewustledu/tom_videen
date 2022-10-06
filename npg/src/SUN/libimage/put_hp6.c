/*  $Id: put_hp6.c,v 1.2 1999/12/13 17:45:50 tom Exp $
 *  $Log: put_hp6.c,v $
 *  Revision 1.2  1999/12/13 17:45:50  tom
 *  tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		put_hp6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Write PETT VI slice header.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/put_hp6.c,v 1.2 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <image.h>

int             put_hp6 (file)
	ImageFile      *file;
{
	if (fwrite (file->p6, 1, RECLEN, file->fp) != RECLEN) {
		(void) fprintf (stderr, "ERROR [put_hp6]: Cannot write PETT VI header for %s\n", file->name);
		return (ERROR);
	}
	return (OK);
}
