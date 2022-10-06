/*$Id: itoa.c,v 1.2 1993/11/03 18:02:48 ty7777 Exp $*/
/*$Log: itoa.c,v $
 * Revision 1.2  1993/11/03  18:02:48  ty7777
 * Added rcsid.
 **/

/*__________________________________________________________________
 *
 *	File Name:	itoa.c
 *	Function Name:	void itoa (int n, char s[])
 *	Arguments:	n:	an integer to be converted.
 *			string:	string converted from the integer n.
 *	Return Value:	none
 *	Description:	Converts an integer to a string.
 *	Reference:
 *	   	   Brian W. Kernighan and Dennis M. Ritchie,
 *		   "The C Programming Language", Prentice Hall, 1988,
 *	   	   pp. 62--64.
 *
 *	Author:	Tom Yang
 *	Date:	04/08/1992	
 *
 __________________________________________________________________*/

#include <string.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libnpg/RCS/itoa.c,v 1.2 1993/11/03 18:02:48 ty7777 Exp $";

void reverse ();

void itoa (n, string)
	int	n;
	char	*string;
{
	int	i, sign;

	/*
	 * Record sign and make n positive if necessary. 
	 */
	if ((sign = n) < 0)	
		n	*= -1;

	i	= 0;

	/*
	 * Generate digits in reverse order.
	 */
	do
	{
		string [i++]	= n % 10 + '0';
	} while ((n /= 10) > 0);

	if (sign < 0)
		string [i++]	= '-';
	string [i]	= '\0';
	reverse (string);
}

/*___________________________________________________________________

 *	Function Name:	void reverse (char string[])
 *	Arguments:	string:	string to be reversed.
 *	Return Value:	none
 *	Description:	Reverse string in place.
 __________________________________________________________________*/
void reverse (string)
	char	*string;
{
	int	c, i, j;

	for (i = 0, j = strlen (string) - 1; i < j; i++, j--)
	{
		c		= string [i];
		string [i]	= string [j];
		string [j]	= c;
	}
}
