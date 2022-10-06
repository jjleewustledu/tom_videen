#include <time.h>

#ifdef HP
void            getime (datearray)
#else
void            getime_ (datearray)
#endif
	long            datearray[];
{
	struct tm      *t;
	long            l;

	l = time ((long *) 0);
	t = localtime (&l);
	datearray[0] = t->tm_hour;
	datearray[1] = t->tm_min;
	datearray[2] = t->tm_sec;
}
