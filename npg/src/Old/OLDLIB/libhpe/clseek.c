#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef HP
void            clseek (fildes, offset, whence)
#else
void            clseek_ (fildes, offset, whence)
#endif
	int            *fildes;
	off_t          *offset;
	int            *whence;
{
	if (lseek (*fildes, *offset, *whence) == -1) {
		fprintf (stderr, "\nError in procedure clseek.\n");
		perror ("lseek");
		exit (1);
	}
}
