/*  $Id: make_imgfile.c,v 1.4 2000/09/27 19:03:28 tom Exp $
    $Log: make_imgfile.c,v $
 * Revision 1.4  2000/09/27  19:03:28  tom
 * *** empty log message ***
 *
 * Revision 1.3  2000/07/18  15:40:32  tom
 * add Interfile compatibility
 *
 * Revision 1.2  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		make_imgfile
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Malloc an ImageFile
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/make_imgfile.c,v 1.4 2000/09/27 19:03:28 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>

ImageFile      *make_imgfile (fname)
	char           *fname;
{
	ImageFile      *file;

	file = (ImageFile *) calloc (1, sizeof (ImageFile));
	file->name = (char *) calloc (MAXPATH, sizeof (char));
	(void) strcpy (file->name, fname);
	file->fp = NULL;
	(void) strcpy (file->mode, "r");
	file->mh = NULL;
	file->sh = NULL;
	file->ih = NULL;
	file->m6 = NULL;
	file->m7 = NULL;
	file->p6 = NULL;
	return (file);
}
