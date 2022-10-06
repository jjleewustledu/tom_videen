/*$Id: Lowered.c,v 1.1 1994/04/07 15:28:15 ty7777 Exp $*/
/*$Log: Lowered.c,v $
 * Revision 1.1  1994/04/07  15:28:15  ty7777
 * Initial revision
 **/

/*____________________________________________________________________________________	
	File:		Lowered.c.

	Function:	void Lowered (char *string, int length).

	Arguments:	string,	string to be lowered.
			length,	length of string.

	Description:	Convert a character string to lower case.

	Author:		Tom Yang on Dec. 8, 1991.
___________________________________________________________________________________*/

#include <petutil/petutil.h>

static char rcsid [] = "$Header: /home/petsun4/ty7777/src/libnpg/RCS/Lowered.c,v 1.1 1994/04/07 15:28:15 ty7777 Exp $";

PUBLIC void Lowered (string, length)
	char	*string;
	int	length;
{
	int	i;

	for (i = 0; i < length; i++)
		string [i] = tolower (*(string + i));
}
