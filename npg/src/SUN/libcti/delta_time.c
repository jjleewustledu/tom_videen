/*$Id: delta_time.c,v 1.2 1996/09/25 16:28:32 tom Exp $*/
/*$Log: delta_time.c,v $
 *Revision 1.2  1996/09/25 16:28:32  tom
 *build in Solaris 2
 *
 * Revision 1.1  1994/03/11  20:41:04  ty7777
 * Initial revision
 **/

#include <sys/time.h>

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libcti/RCS/delta_time.c,v 1.2 1996/09/25 16:28:32 tom Exp $";
#endif

delta_time (t1, t0)
	struct timeval  t1, t0;
{
	int             etime, dsec, dusec;

	dsec = t1.tv_sec - t0.tv_sec;
	dusec = t1.tv_usec - t0.tv_usec;
	etime = dsec * 1000 + dusec / 1000;
	return (etime);
}
