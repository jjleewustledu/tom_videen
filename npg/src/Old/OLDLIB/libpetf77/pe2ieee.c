/*$Id: pe2ieee.c,v 1.1 1995/10/10 15:43:22 ty7777 Exp $*/
/*$Log: pe2ieee.c,v $
 * Revision 1.1  1995/10/10  15:43:22  ty7777
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
 *	Author:			Unknown
 *	Source:			Gary Hoffman
 *
_______________________________________________________________*/  

#include <math.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libpetf77/RCS/pe2ieee.c,v 1.1 1995/10/10 15:43:22 ty7777 Exp $";

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
