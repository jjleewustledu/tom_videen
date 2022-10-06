/*  $Id: get_mh6.c,v 1.2 1999/12/13 17:45:50 tom Exp $
 *	$Log: get_mh6.c,v $
 *	Revision 1.2  1999/12/13 17:45:50  tom
 *	tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		get_mh6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Read Version 6 ECAT main header
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/get_mh6.c,v 1.2 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             get_mh6 (file)
	ImageFile      *file;
{

	if (file->m6->mh == NULL) {
		if ((file->m6->mh = (Main_header *) malloc (sizeof (Main_header))) == NULL) {
			(void) fprintf (stderr, "ERROR [get_mh6]: Could not malloc memory for m6->mh\n");
			return (ERROR);
		}
	}
	if (mat_read_main_header (file->m6->mfp->fptr, file->m6->mh) != 0) {
		(void) fprintf (stderr, "ERROR [get_mh6]: Could not read mainheader for %s\n",
			 file->name);
		return (ERROR);
	}
	return (OK);
}
