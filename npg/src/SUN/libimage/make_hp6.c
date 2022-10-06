/*  $Id: make_hp6.c,v 1.2 1999/12/13 17:45:50 tom Exp $
 *  $Log: make_hp6.c,v $
 *  Revision 1.2  1999/12/13 17:45:50  tom
 *  tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		make_hp6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Allocate memory for PETT VI header.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/make_hp6.c,v 1.2 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             make_hp6 (file)
	ImageFile      *file;
{
	if ((file->p6 = (PETT6HDR *) malloc (sizeof (PETT6HDR))) == NULL) {
		(void) fprintf (stderr, "ERROR [make_hp6]: cannot malloc PETT6HDR\n");
		return (ERROR);
	}
	return (OK);
}
