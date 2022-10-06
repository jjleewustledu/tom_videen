/*  $Id: make_sh7.c,v 1.3 2000/09/27 19:03:28 tom Exp $
    $Log: make_sh7.c,v $
 * Revision 1.3  2000/09/27  19:03:28  tom
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
 *	Function:		make_sh7
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Allocate a Version 7 image subheader.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/make_sh7.c,v 1.3 2000/09/27 19:03:28 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             make_sh7 (file)
	ImageFile      *file;
{

	if (file->m7->sh == NULL) {
		if ((file->m7->sh = (Image_subheader7 *) calloc (1, sizeof (Image_subheader7))) == NULL) {
			(void) fprintf (stderr, "ERROR [make_sh7]: cannot calloc m7->sh\n");
			return (ERROR);
		}
	}
	file->m7->sh->data_type = 0;
	return (OK);
}
