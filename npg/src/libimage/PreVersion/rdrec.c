/*  $Id: rdrec.c,v 1.3 2002/12/19 17:37:41 tom Exp $
    $Log: rdrec.c,v $
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
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Read <length> records starting at record number <recnum> from
 *					disk file specified by <fd> into <buffer>.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/rdrec.c,v 1.3 2002/12/19 17:37:41 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <image.h>

int             rdrec (fd, buffer, recnum, length)
	FILE           *fd;			 /* File pointer	 */
	char           *buffer;		 /* Input buffer	 */
	int             recnum;		 /* Beginning record number	(First record = 1) */
	int             length;		 /* Number of records to read	 */
{
	long            dskptr;
	int             count, stat;

/* Position pointer beginning of record. */

	dskptr = (recnum - 1) * LENREC;
	stat = fseek (fd, (long) dskptr, SEEK_SET);
	if (stat != 0)
		return (ERROR);

/* Read next record.    */

	count = length * LENREC;
	stat = fread (buffer, sizeof (char), count, fd);
	if (stat != count) {
		(void) fprintf (stderr, "ERROR [rdrec]: Read %d of %d records\n", stat, count);
		return (ERROR);
	} else
		return (OK);
}
