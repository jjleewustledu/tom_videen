/*	$Id: readhdr.c,v 3.0 2003/08/21 15:16:40 tom Exp $
	$Log: readhdr.c,v $
 * Revision 3.0  2003/08/21  15:16:40  tom
 * 21Aug2003
 *
 * Revision 2.2  2000/07/18  15:57:37  tom
 * remove error message
 *
 * Revision 2.1  1996/05/16  19:44:00  tom
 * check existence of fp before writing
 *
 * Revision 2.0  1996/04/24  23:07:41  tom
 * new library release
 *
 */
/* ===============================================================================
 *	Module:			readhdr.c
 *	Date:			Apr-96
 *	Author:			Tom Videen
 *	Description:	Read an 'hdr' file into HEADER_DATA.
 * ===============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/readhdr.c,v 3.0 2003/08/21 15:16:40 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <imgutil.h>

int             readhdr (fname, hdr, fp)
	char           *fname;			 /* image file name */
	HEADER_DATA    *hdr;			 /* hdr structure */
	FILE           *fp;			 /* rec file pointer */
{
	char           *hdrname;		 /* hdr fname */
	char           *local_hdrname;	 /* local hdr fname */
	char           *c;

	hdrname = (char *) calloc (MAXPATH, sizeof (char));
	local_hdrname = (char *) calloc (MAXPATH, sizeof (char));
	(void) strcpy (hdrname, fname);
	c = strrchr (hdrname, '.');
	if (c == NULL)
		c = strrchr (hdrname, '\0');
	(void) strcpy (c, ".hdr");

	c = strrchr (hdrname, '/');
	if (c == NULL)
		(void) strcpy (local_hdrname, hdrname);
	else
		(void) strcpy (local_hdrname, c + 1);

	if (gethdr (local_hdrname, hdr) == 0) {
		fprintf (stdout, "HDR = %s\n", local_hdrname);
		if (fp != NULL)
			fprintf (fp, "%s\n", local_hdrname);
	} else if (gethdr (hdrname, hdr) == 0) {
		fprintf (stdout, "HDR = %s\n", hdrname);
		if (fp != NULL)
			fprintf (fp, "%s\n", hdrname);
	} else {
		free (hdrname);
		free (local_hdrname);
		return (ERROR);
	}
	free (hdrname);
	free (local_hdrname);
	return (OK);
}
