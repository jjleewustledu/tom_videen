/*	$Id: gethdrpath.c,v 2.0 1996/04/24 23:07:41 tom Exp $
	$Log: gethdrpath.c,v $
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
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutilOLD/RCS/gethdrpath.c,v 2.0 1996/04/24 23:07:41 tom Exp $";
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

	strncpy (complete_hdrfile, "\0", MAXLINE);
	if (extension != NULL) {
		cbuffer = extension + 1;
		strncpy (complete_hdrfile, imgfile, strlen (imgfile) - strlen (extension) + 1);
	} else
		cbuffer = imgfile;
	strcpy (short_imgfile, cbuffer);

	strncpy (short_hdrfile, "\0", MAXLINE);
	extension = strrchr (short_imgfile, '.');
	if (extension != NULL)
		strncpy (short_hdrfile, short_imgfile, strlen (short_imgfile) - strlen (extension));
	else
		strcpy (short_hdrfile, short_imgfile);

	strcat (short_hdrfile, HDR_EXT);
	strcat (complete_hdrfile, short_hdrfile);
	return;
}
