/*$Id: intdate.c,v 1.1 1995/10/10 15:38:10 ty7777 Exp $*/
/*$Log: intdate.c,v $
 * Revision 1.1  1995/10/10  15:38:10  ty7777
 * Initial revision
 **/

#include <time.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libpetf77/RCS/intdate.c,v 1.1 1995/10/10 15:38:10 ty7777 Exp $";

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
