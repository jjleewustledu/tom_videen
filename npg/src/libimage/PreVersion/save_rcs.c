/*	$Id: save_rcs.c,v 1.4 2002/12/17 21:55:15 tom Exp $
 *	$Log: save_rcs.c,v $
 * Revision 1.4  2002/12/17  21:55:15  tom
 * fix pointer/cast errors found with gcc
 *
 *	Revision 1.3  2000/09/28 14:44:29  tom
 *	calloc
 *
 * Revision 1.2  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* ===============================================================================
 *	Module:			save_rcs.c
 *	Date:			Nov-95
 *	Author:			Tom Videen
 *	Description:	Store RCS version in file->mh->sw_revision and date + uid in file->mh->filetype.
 * ===============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/save_rcs.c,v 1.4 2002/12/17 21:55:15 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>

int             save_rcs (file, version, runstamp)
	ImageFile      *file;
	char           *version;
	char           *runstamp;
{
	char           *ptr, *ptr2;
	int             len;

	ptr = strstr (version, "RCS") + 4;
	if (ptr == NULL)
		return (ERROR);
	ptr2 = strrchr (version, '/') + 3;
	if (ptr2 == NULL)
		return (ERROR);
	ptr2 = NULL;
	free (file->mh->sw_revision);
	len = strlen (ptr) + 1;
	file->mh->sw_revision = (char *) calloc (len, sizeof (char));
	(void) strcpy (file->mh->sw_revision, ptr);

	free (file->mh->filetime);
	len = strlen (runstamp) + 1;
	file->mh->filetime = (char *) calloc (len, sizeof (char));
	(void) strcpy (file->mh->filetime, runstamp);
	return (OK);
}
