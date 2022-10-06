/*	$Id: stringtype.c,v 2.1 1999/12/13 17:04:17 tom Exp $
	$Log: stringtype.c,v $
 * Revision 2.1  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 2.0  1996/04/24  23:07:41  tom
 * new library release
 *
 * Revision 1.1  1996/03/12  23:00:34  tom
 * Initial revision
 * */
/* ===============================================================================
 *	Module:			stringtype.c
 *	Date:			Jul-93
 *	Author:			Tom Yang
 *	Description: 	Determine the data type contained in a string.
 *	Algorithm:
 *			if (there is at least one letter in the string) then
 *				return string type
 *			else if (string contains no decimal point) then
 *				return integer type
 *			else
 *				return real type
 *			endif
 * ===============================================================================
 */
#ifndef lint
static char		rcsid[] = "$Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/stringtype.c,v 2.1 1999/12/13 17:04:17 tom Exp $";
#endif 

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <imgutil.h>

int             stringtype (string)
	char           *string;
{
	int             i;
	int             string_length;

	string_length = strlen (string);

	for (i = 0; i < string_length; i++)
		if (isalpha (string[i]))
			break;

	if (i < string_length)
		return TYPE_STRING;
	else if (strchr (string, '.') == NULL)
		return TYPE_INTEGER;
	else
		return TYPE_REAL;
}
