/*	$Id: gethdrpath.c,v 3.0 2003/08/21 15:16:40 tom Exp $
	$Log: gethdrpath.c,v $
 * Revision 3.0  2003/08/21  15:16:40  tom
 * 21Aug2003
 *
 * Revision 2.0  1996/04/24  23:07:41  tom
 * new library release
 *
 * Revision 1.1  1996/03/12  23:00:34  tom
 * Initial revision
 * */
/* ===============================================================================
 *	Function:		gethdrpath.c
 *	Description:	Get hdr filename
 * ===============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/gethdrpath.c,v 3.0 2003/08/21 15:16:40 tom Exp $";
#endif

#include <stdlib.h>
#include <string.h>
#include <myvalues.h>		/* MAXLINE */
#include <imgutil.h>		/* HDR_EXT */

void             gethdrpath (imgfile, short_hdrfile, complete_hdrfile)
	char           *imgfile;			/* input */
	char           *short_hdrfile;		/* returned */
	char           *complete_hdrfile;	/* returned */
{
	char           *cbuffer;
	char           *extension;
	char            short_imgfile[MAXLINE];

	extension = strrchr (imgfile, '/');

	(void) strncpy (complete_hdrfile, "\0", MAXLINE);
	if (extension != NULL) {
		cbuffer = extension + 1;
		(void) strncpy (complete_hdrfile, imgfile, strlen (imgfile) - strlen (extension) + 1);
	} else
		cbuffer = imgfile;
	(void) strcpy (short_imgfile, cbuffer);

	(void) strncpy (short_hdrfile, "\0", MAXLINE);
	extension = strrchr (short_imgfile, '.');
	if (extension != NULL)
		(void) strncpy (short_hdrfile, short_imgfile, strlen (short_imgfile) - strlen (extension));
	else
		(void) strcpy (short_hdrfile, short_imgfile);

	(void) strcat (short_hdrfile, HDR_EXT);
	(void) strcat (complete_hdrfile, short_hdrfile);
	return;
}
