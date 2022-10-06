/*  $Id: rdrec.c,v 1.4 2003/07/01 19:14:19 tom Exp $
    $Log: rdrec.c,v $
 * Revision 1.4  2003/07/01  19:14:19  tom
 * *** empty log message ***
 *
 * Revision 1.3  2002/12/19  17:37:41  tom
 * add stat
 *
 * Revision 1.2  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		rdrec
 *  Date:           Mar 2003
 *  Author:         Tom Videen
 *	Description:	Read nrec records starting at recnum from fd into buffer.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/rdrec.c,v 1.4 2003/07/01 19:14:19 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <image.h>

int             rdrec (fd, buffer, recnum, nrec)
	FILE           *fd;			 /* File pointer	 */
	char           *buffer;			 /* Input buffer	 */
	int             recnum;			 /* Beginning record number	(First record = 1) */
	int             nrec;			 /* Number of records to read	 */
{
	long            dskptr;
	int             nbytes, stat;

	dskptr = (recnum - 1) * LENREC;
	stat = fseek (fd, dskptr, SEEK_SET);
	if (stat != 0) {
		(void) fprintf (stderr, "ERROR [rdrec]: fseek %d\n", stat);
		return (ERROR);
	}
	nbytes = nrec * LENREC;
	stat = fread (buffer, sizeof (char), nbytes, fd);
	if (stat != nbytes) {
		(void) fprintf (stderr, "ERROR [rdrec]: fread %d\n", stat);
		return (ERROR);
	} else
		return (OK);
}
