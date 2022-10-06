/*________________________________________________________________________________
File:		determine_type.c

Algorithm:	if (there is at least one letter in the string) then
			return string type
		else if (string contains no decimal point) then
			return integer type
		else
			return real type
		endif

Description:	Determine the data type contained in a string.

Author:		Tom Yang on July 22, 1993.
________________________________________________________________________________*/

#include <petutil.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libnpg/RCS/determine_type.c,v 1.4 1995/11/10 16:28:06 tom Exp $";
 
PUBLIC int DetermineType (string)
	char	*string;
{
	int	i;
	int	string_length;

	string_length	= strlen (string);

	for (i = 0; i < string_length; i++)
		if (isalpha (string [i]))
			break;

	if (i < string_length)
		return DATA_TYPE_STRING;
	else if (strchr (string, '.') == NULL)
		return DATA_TYPE_INTEGER;
	else
		return DATA_TYPE_REAL;
}
