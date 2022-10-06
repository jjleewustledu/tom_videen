/*	$Id: readhdr.c,v 2.2 2000/07/18 15:57:37 tom Exp $
	$Log: readhdr.c,v $
 * Revision 2.2  2000/07/18  15:57:37  tom
 * remove error message
 *
 * Revision 2.1  1996/05/16  19:44:00  tom
 * check existence of recfp before writing
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
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutilOLD/RCS/readhdr.c,v 2.2 2000/07/18 15:57:37 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <imgutil.h>

int             readhdr (filename, hdr, recfp)
	char           *filename;		 /* image file name */
	HEADER_DATA    *hdr;			 /* hdr structure */
	FILE           *recfp;			 /* rec file pointer */
{
	char           *hdrname;		 /* hdr filename */
	char           *local_hdrname;	 /* local hdr filename */
	char           *c;

	hdrname = (char *) calloc (MAXPATH, sizeof (char));
	local_hdrname = (char *) calloc (MAXPATH, sizeof (char));
	strcpy (hdrname, filename);
	c = strrchr (hdrname, '.');
	if (c == NULL)
		c = strrchr (hdrname, '\0');
	strcpy (c, ".hdr");

	c = strrchr (hdrname, '/');
	if (c == NULL)
		strcpy (local_hdrname, hdrname);
	else
		strcpy (local_hdrname, c + 1);

	if (gethdr (local_hdrname, hdr) == 0) {
		printf ("HDR = %s\n", local_hdrname);
		if (recfp != NULL)
			fprintf (recfp, "%s\n", local_hdrname);
	} else if (gethdr (hdrname, hdr) == 0) {
		printf ("HDR = %s\n", hdrname);
		if (recfp != NULL)
			fprintf (recfp, "%s\n", hdrname);
	} else {
		/* printf ("ERROR [readhdr]:  hdrfile not found as %s or %s\n", local_hdrname, hdrname); */
		free (hdrname);
		free (local_hdrname);
		return (ERROR);
	}
	free (hdrname);
	free (local_hdrname);
	return (OK);
}
