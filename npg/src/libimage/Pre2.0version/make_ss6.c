/*  $Id: make_ss6.c,v 1.1 2000/02/18 21:49:13 tom Exp $
    $Log: make_ss6.c,v $
 * Revision 1.1  2000/02/18  21:49:13  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		make_ss6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Allocate a Version 6 sinogram subheader.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/make_ss6.c,v 1.1 2000/02/18 21:49:13 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             make_ss6 (file)
	ImageFile      *file;
{

	if (file->m6->scn == NULL) {
		if ((file->m6->scn = (Scan_subheader *) malloc (sizeof (Scan_subheader))) == NULL) {
			(void) fprintf (stderr, "ERROR [make_ss6]: cannot malloc m6->scn\n");
			return (ERROR);
		}
	}
	file->m6->scn->data_type = 0;
	return (OK);
}
