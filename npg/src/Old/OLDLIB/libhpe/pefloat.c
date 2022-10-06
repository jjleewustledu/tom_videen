/*________________________________________________________________
 *	pefloat.c
 *
 *	Converts Perkin Elmer 4 byte floating point numbers
 *	to IEEE floating point representation.
 *
 *	Author: Unknown
 *	Source: Gary Hoffman
 *
 *	Changed the type of f from unsigned int to int and changed
 *	function name to make it compatible with Sun Fortran.
 *	Date:	Oct. 2, 1991
 *	By:	Tom Yang
 _______________________________________________________________*/

#include <math.h>

#ifdef HP
float pefloat (f)
#else
float pefloat_ (f)
#endif
	int            *f;			 /* 4 byte float from OS32 */
{
	float           c;
	int             j;
	int             iexp;
	int             idata;
	double          value;
	double          pow ();

	idata = *f;
	if (idata != 0) {
		j = idata & 0xffffff;
		if ((idata & 0x80000000) != 0)
			j = -j;
		iexp = (idata >> 24) & 0x7f;
		value = (double) j *pow ((double) 16, (double) (iexp - 0x46));

		c = (float) value;
	} else
		c = 0.0;
	return (c);			 /* return 4 byte IEEE float for UNIX */
}
