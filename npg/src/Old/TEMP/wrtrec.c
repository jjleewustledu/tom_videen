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
 *  Date:           Mar 2003
 *  Author:         Tom Videen
 *	Description:	Write nrec records starting at recnum from buffer into fd.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/wrtrec.c,v 1.2 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <image.h>

int             wrtrec (fd, buffer, recnum, nrec)
	FILE           *fd;			 /* File pointer */
	char           *buffer;			 /* Input buffer */
	int             recnum;			 /* Beginning record number */
	int             nrec;			 /* Number of records to write */
{
	long            dskptr;
	int             nbytes, stat;

	dskptr = (recnum - 1) * LENREC;
	stat = fseek (fd, dskptr, SEEK_SET);
	if (stat != 0) {
		(void) fprintf (stderr, "ERROR [wrtrec]: fseek %d\n", stat);
		return (ERROR);
	}
	nbytes = nrec * LENREC;
	if ((stat = fwrite (buffer, sizeof (char), nbytes, fd)) != nbytes) {
		(void) fprintf (stderr, "ERROR [wrtrec]: fread %d\n", stat);
		return (ERROR);
	} else
		return (OK);
}
