#include <stdio.h>
#include <string.h>

#ifdef HP
prntfile (filename)
#else
prntfile_ (filename)
#endif
	char           *filename;
{
	int             stat;
	char            command[80];

	strcpy (command, "enscript ");
	strcat (command, filename);
	stat = system (command);
}
