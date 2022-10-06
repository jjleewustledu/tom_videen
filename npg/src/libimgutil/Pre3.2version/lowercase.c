/*	$Id: lowercase.c,v 2.1 1999/12/13 17:04:17 tom Exp $
	$Log: lowercase.c,v $
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
 *	Module:			lowercase.c
 *	Date:			Dec-91
 *	Author:			Tom Yang
 *	Description:	Convert a character string to lower case
 * ===============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/lowercase.c,v 2.1 1999/12/13 17:04:17 tom Exp $";
#endif

#include <ctype.h>
#include <string.h>

void            lowercase (string)
	char           *string;			 /* string to convert */
{
	int             i;

	for (i = 0; i < strlen(string); i++)
		string[i] = tolower (*(string + i));
}
