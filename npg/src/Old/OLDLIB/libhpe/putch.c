#include <stdio.h>
#include <fcntl.h>

#ifdef HP
void            putch (ch)
#else
void            putch_ (ch)
#endif
	char           *ch;
{
	putchar (*ch);
	fflush (stdout);
}
