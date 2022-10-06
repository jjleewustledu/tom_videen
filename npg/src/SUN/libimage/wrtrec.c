/*  $Id: wrtrec.c,v 1.2 1999/12/13 17:45:50 tom Exp $
    $Log: wrtrec.c,v $
    Revision 1.2  1999/12/13 17:45:50  tom
    tov revision

 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		wrtrec
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Write <length> records starting at record number <recnum> from
 *					disk file specified by <fd> into <buffer>.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/wrtrec.c,v 1.2 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <image.h>

int             wrtrec (fd, buffer, recnum, length)
	FILE           *fd;			 	/* File pointer */
	char           *buffer;			/* Input buffer */
	int             recnum;			/* Beginning record number */
	int             length;			/* Number of records to write */
{
	long            dskptr;
	int             count, stat;

/* Position pointer beginning of record */

	dskptr = (recnum - 1) * LENREC;
	if (fseek (fd, (long) dskptr, SEEK_SET) != 0)
		return (ERROR);

/* Write next record */

	count = length * LENREC;
	if ((stat = fwrite (buffer, sizeof (char), count, fd)) != count) {
		(void) fprintf (stderr, "ERROR [wrtrec]: Wrote %d of %d records\n", stat, count);
		return (ERROR);
	} else
		return (OK);
}
