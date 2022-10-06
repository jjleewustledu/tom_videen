/*$Log: intdate.c,v $
 * Revision 1.2  1999/12/15  21:19:57  tom
 * y2k fix for localtime
 *
 * Revision 1.1  1996/09/26  20:18:38  tom
 * Initial revision
 *
 * Revision 1.2  1994/03/03  20:26:31  ty7777
 * Replace id with header.
 *
 * Revision 1.1  1993/09/09  19:31:58  ty7777
 * Initial revision
 **/

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/intdate.c,v 1.2 1999/12/15 21:19:57 tom Exp $";
#endif

#include <time.h>

void            intdate (datearray)
	long            datearray[];
{
	struct tm      *t;
	long            l;

	l = time ((long *) 0);
	t = localtime (&l);
	datearray[0] = t->tm_year;
	if (datearray[0] < 1000)
		datearray[0] += 1900;
	datearray[1] = t->tm_mon + 1;
	datearray[2] = t->tm_mday;
}
