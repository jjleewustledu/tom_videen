#include <stdlib.h>

#ifdef HP
int             newctoi (string)
#else
int             newctoi_ (string)
#endif
	char           *string;
{
	int             value;

	value = atoi (string);
	return value;
}
