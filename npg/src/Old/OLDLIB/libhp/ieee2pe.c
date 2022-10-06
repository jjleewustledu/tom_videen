/*$Id: ieee2pe.c,v 1.3 1994/03/03 20:24:30 ty7777 Exp $*/
/*$Log: ieee2pe.c,v $
 * Revision 1.3  1994/03/03  20:24:30  ty7777
 * Replace id with header.
 **/

/*__________________________________________________________
 *	ieee2pe.c
 *
 *	Converts IEEE floating point numbers to Perkin Elmer 4 
 *	byte floating point representation used for OS32. 
 *	
 *	References:
 *			PE			IEEE
 *	sign		1			  1
 *	exp		7			  8
 *	mantisa		24			 23
 *	float:	m * 16 ** (e - 70)	m * 2 ** (e - 127)
 *	Please see:
 *	1. ``IEEE Standard for Binary Floating-Point Arithemetic'',
 *	   ANSI/IEEE Std 754-1985, American National Standards
 *	   Institute, pp 8--10.
 *	2. Mumerical Computation Guide, Sun Microsystems, pp. 3--11.
 *	3. Model 3230 Processor, Perkin-Elmer, computer Systems
 *	   Division, 2 Crescent Place, Oceanport, NJ 07757,
 *	   pp. 6-3 -- 6-4.
 *
 *	Formula:
 *	IEEE	f = m * 2 ** (e - 127)
 *		  = m * 2 ** (4 (e / 4)) * 2 ** (e % 4 - 127)
 *		  = m * 16 ** ( e / 4 - 32) * 2 ** (e % 4 + 1)
 *		  = m * 16 ** (e / 4 + 38 - 70) * 2 ** ( e % 4 + 1)
 *		  = (m * 2 ** (e % 4 - 3)) * 16 ** ((e / 4 + 39) - 70)
 *
 *	With C function frexp, we have:
 *	f = m * 2 ** e = m * 2 ** ((e + 127) - 127),
 *	hence
 *	M = m * 2 ** ((e + 127) % 4 - 3),
 *	E = (e + 127) / 4 + 39.
 *
 *	Author: Tom Yang
 *	Date:	01/27/92	
 __________________________________________________________*/

#include <math.h>
#define	IEEE_BIAS	127
#define	FLOAT_SIZE	32		/* 4 bytes, 32 bits */
#define	PE_SIGN_POS	32		
#define	PE_MANTISSA	24		

static char rcsid []= "$Header: /home/npggw/tom/src/libhp/RCS/ieee2pe.c,v 1.3 1994/03/03 20:24:30 ty7777 Exp $";

int ieee2pe (pef)
float 	*pef;		/* 4 byte float from SUN IEEE */
{
	int	pe_representation;
	float	decimal;
	int	mantissa;
	int	ieee_exp;
	int	exp;
	int	sign;
	int	temp;

	decimal	= frexp (*pef, &ieee_exp);
	if (decimal < 0.0)
	{
		sign	= 1;
		decimal	*= -1;
	}
	else
		sign	= 0;

	/* Shift the decimal left by PE_MANTISSA bits */
	decimal	= (double)decimal * ldexp (1.0, PE_MANTISSA);

	/* Rounding */
	mantissa	= (int) (decimal + 0.5);

	/* To make the product od mantissa and exp part unchanged,
	   we substract 5 from the exponent. */
	exp		= (ieee_exp + IEEE_BIAS) / 4 + 39 - PE_MANTISSA / 4;

	/* The part of the exponent which can not be put in 
	   the Perkin-Elmer exponent. We form the new mantissa
	   from the old mantissa and the remainder. The variable
	   "temp" is always less than or equal to 0 */
	temp		= (ieee_exp + IEEE_BIAS) % 4 - 3;
	mantissa	= mantissa >> (-temp);

	/* Perkin-Elmer float number representation */
	pe_representation	= (sign << PE_SIGN_POS - 1) 
					+ (exp << PE_MANTISSA) + mantissa; 

	/* return 4 byte PE float representation for OS32 */
	return (pe_representation);	
}

