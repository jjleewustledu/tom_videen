/*------------------------------------------------------------------
	Calling Function:	sysio.f
------------------------------------------------------------------*/
#include <stdio.h>

#ifdef HP
void            cwrite_ (fildes, buf, nbyte)
#else
void            cwrite_ (fildes, buf, nbyte)
#endif
	int            *fildes;
	char           *buf;
	unsigned       *nbyte;
{
	if (write (*fildes, buf, *nbyte) == -1) {
		fprintf (stderr, "\nError in procedure cwrite.\n");
		perror ("write");
		exit (1);
	}
}
