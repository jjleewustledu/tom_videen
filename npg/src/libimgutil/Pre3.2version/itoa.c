/*	$Id: itoa.c,v 2.1 1999/12/13 17:04:17 tom Exp $
	$Log: itoa.c,v $
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
 *	Module:			itoa.c
 *	Date:			Apr-92
 *	Author:			Tom Yang
 *	Description:	Convert integer to string.
 *	Reference:
 *	   	   Brian W. Kernighan and Dennis M. Ritchie,
 *		   "The C Programming Language", Prentice Hall, 1988,
 *	   	   pp. 62--64.
 * ===============================================================================
 */
#ifndef lint
static char		rcsid[] = "$Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/itoa.c,v 2.1 1999/12/13 17:04:17 tom Exp $";
#endif 

#include <string.h>

/*___________________________________________________________________

 *	Function Name:	void reverse (char string[])
 *	Arguments:	string:	string to be reversed.
 *	Return Value:	none
 *	Description:	Reverse string in place.
 __________________________________________________________________*/
static void            reverse (string)
	char           *string;
{
	int             c, i, j;

	for (i = 0, j = strlen (string) - 1; i < j; i++, j--) {
		c = string[i];
		string[i] = string[j];
		string[j] = c;
	}
}

void            itoa (n, string)
	int             n;
	char           *string;
{
	void            reverse ();
	int             i, sign;

/*
 * Record sign and make n positive if necessary.
 */
	if ((sign = n) < 0)
		n *= -1;
	i = 0;

/*
 * Generate digits in reverse order.
 */
	do {
		string[i++] = n % 10 + '0';
	} while ((n /= 10) > 0);

	if (sign < 0)
		string[i++] = '-';
	string[i] = '\0';
	reverse (string);
}
