/*  $Id: make_mh7.c,v 1.4 2000/09/27 19:03:28 tom Exp $
    $Log: make_mh7.c,v $
 * Revision 1.4  2000/09/27  19:03:28  tom
 * *** empty log message ***
 *
 * Revision 1.3  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.2  1997/05/14  19:16:48  tom
 * check for defined file->m7
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		make_mh7
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Allocate and fill a Version 7 main header
 *					using fields from the generic MainHeader.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/make_mh7.c,v 1.4 2000/09/27 19:03:28 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>

int             make_mh7 (file)
	ImageFile      *file;
{

    if (file->m7 == NULL) {
        if ((file->m7 = (M7header *) calloc (1, sizeof (M7header))) == NULL) {
            (void) fprintf (stderr, "ERROR [make_mh7]: calloc m7 error for %s\n", file->name);
            return (ERROR);
        }
        file->m7->mh = NULL;
        file->m7->sh = NULL;
        file->m7->fsr = NULL;
    }
	if (file->m7->mh == NULL) {
		if ((file->m7->mh = (Main_header7 *) calloc (1, sizeof (Main_header7))) == NULL) {
			(void) fprintf (stderr, "ERROR [make_mh7]: cannot calloc m7->mh\n");
			return (ERROR);
		}
	}
	file->m7->mh->system_type = 0;
	file->m7->mh->file_type = 0;
	return (OK);
}
