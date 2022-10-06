/*	$Id: pe2ieee.c,v 1.1 1996/04/19 19:33:20 tom Exp $
 *	$Log: pe2ieee.c,v $
 *	Revision 1.1  1996/04/19 19:33:20  tom
 *	Initial revision
 *
 */
/* ===============================================================================
 *	Module:			pe2ieee.c
 *	Date:			Jan-92
 *	Author:			Tom Yang
 *	Description:	Converts Perkin Elmer 4 byte floating point number
 *					representation to IEEE floating point representation.
 *	Input:			4-byte OS32 floating point number.
 *	Returns:		4-byte IEEE floating point number.
 * ===============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/pe2ieee.c,v 1.1 1996/04/19 19:33:20 tom Exp $";
#endif

#include <math.h>

float           pe2ieee (frep)
	unsigned int    frep;
{
	float           c;
	int             j;
	int             iexp;
	int             idata;
	double          value;

	idata = (int) frep;
	if (idata != 0) {
		j = idata & 0xffffff;
		if ((idata & 0x80000000) != 0)
			j = -j;
		iexp = (idata >> 24) & 0x7f;
		value = ldexp ((double) j, 4 * (iexp - 0x46));
		c = (float) value;
	} else
		c = 0.0;
	return (c);
}
