/*$Id: intdate.c,v 1.3 1995/10/30 22:39:01 tom Exp $*/
/*$Log: intdate.c,v $
 * Revision 1.3  1995/10/30  22:39:01  tom
 * change include to remove "petutil"
 *
 * Revision 1.2  1994/03/03  20:26:31  ty7777
 * Replace id with header.
 *
 * Revision 1.1  1993/09/09  19:31:58  ty7777
 * Initial revision
 **/

#include <time.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libhp/RCS/intdate.c,v 1.3 1995/10/30 22:39:01 tom Exp $";

void	intdate_ (datearray)
long	datearray [];
{
	struct tm	*t;
	long		l;

	l	= time ((long *) 0);
	t	= localtime (&l);
	datearray [0]	= t->tm_year;
	datearray [1]	= t->tm_mon + 1;
	datearray [2]	= t->tm_mday;
}
