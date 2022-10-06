#include <stdio.h>
#include <string.h>

#ifdef HP
void            chkpath (path, length, exist)
#else
void            chkpath_ (path, length, exist)
#endif

	char           *path;
	int            *length;
	char           *exist;
{
	FILE           *fp;
	char           *purepath;

	purepath = (char *) malloc (*length + 1);
	strncpy (purepath, "\0", *length + 1);
	strncpy (purepath, path, *length);

	if ((fp = fopen (purepath, "r")) == NULL) {
		*exist = 0;
	} else {
		*exist = 1;
	}

	free (purepath);
	fclose (fp);
}
