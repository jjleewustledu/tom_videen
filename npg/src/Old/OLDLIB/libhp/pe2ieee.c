/*$Id: pe2ieee.c,v 1.4 1994/11/17 15:34:24 ty7777 Exp $*/
/*$Log: pe2ieee.c,v $
 * Revision 1.4  1994/11/17  15:34:24  ty7777
 * Modified the documentation.
 *
 * Revision 1.3  1994/03/03  20:35:21  ty7777
 * Replace id with header.
 *
 * Revision 1.2  1993/11/03  18:09:45  ty7777
 * Added rcsid.
 *
 * Revision 1.1  1993/03/03  20:38:39  ty7777
 * Initial revision
 **/


/*_______________________________________________________________  
 *
 *	File Name:		pe2ieee.c
 *	Function Name:		float pe2ieee ()
 *	Argument(s):		frep:	4 byte float from OS32.
 *	Function(s) Called:	none.
 *	Return Value:		an IEEE floating point number.
 *	Description:		Converts Perkin Elmer 4 byte floating point numbers 
 *				representation to IEEE floating point representation.
 *	Author:			Tom Yang
 *	Date:			January 27, 1992.
_______________________________________________________________*/  

#include <math.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libhp/RCS/pe2ieee.c,v 1.4 1994/11/17 15:34:24 ty7777 Exp $";

float 	pe2ieee (frep)
unsigned int 	frep;		
{
	float 	c;
	int 	j; 
	int	iexp; 
	int	idata;
	double 	value; 

	idata	= (int) frep;
	if (idata != 0) 
	{
		j	= idata & 0xffffff;
		if ((idata & 0x80000000) != 0)
			j	= -j;
		iexp	= (idata >> 24) & 0x7f;
		value	= ldexp((double)j, 4 * (iexp - 0x46));
		c	= (float) value;
	}
	else 
		c	= 0.0;

	/* 
	 * Return 4 byte IEEE float for UNIX. 
	 */
	return (c);		
}
