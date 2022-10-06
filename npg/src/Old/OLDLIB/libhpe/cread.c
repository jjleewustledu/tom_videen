#include <stdio.h>

#ifdef HP
void            cread (fildes, buf, nbyte)
#else
void            cread_ (fildes, buf, nbyte)
#endif
	int            *fildes;
	char           *buf;
	unsigned       *nbyte;
{
	if (read (*fildes, buf, *nbyte) == -1) {
		fprintf (stderr, "\nError in procedure cread.\n");
		perror ("read");
		exit (1);
	}
}
