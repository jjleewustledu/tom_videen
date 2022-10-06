/*$Id: hd6tot88.c,v 1.4 1995/10/30 22:39:01 tom Exp $*/
/*$Log: hd6tot88.c,v $
 * Revision 1.4  1995/10/30  22:39:01  tom
 * change include to remove "petutil"
 *
 * Revision 1.3  1993/11/03  18:00:25  ty7777
 * Added rcsid.
 *
 * Revision 1.2  1993/07/28  18:44:51  ty7777
 * Change formula for y.
 *
 * Revision 1.1  1993/07/14  19:18:38  ty7777
 * Initial revision
 **/

/*___________________________________________________________________________________
File Name:	hd6tot88.c

Function:	void hd6tot88 (int *x, int *y, int *z)

Arguments:	x:	x coordinate in HD6 space as input, and x coordinate in the
			Talairach 88 space after this function is called.
		y:	y coordinate in HD6 space as input, and y coordinate in the
			Talairach 88 space after this function is called.
		z:	z coordinate in HD6 space as input, and z coordinate in the
			Talairach 88 space after this function is called.
								
Return:		None.

Algorithm:

Description:	Convert coodinates in HD6 space to Talairach 88 space.

Called by:		

Calling:			

Author:		Tom Yang 

History:	Created by Tom Yang on 06/08/1993

___________________________________________________________________________________*/

#include <petutil.h>	

static char rcsid [] = "$Header: /home/npggw/tom/src/libhp/RCS/hd6tot88.c,v 1.4 1995/10/30 22:39:01 tom Exp $";

PUBLIC void hd6tot88 (x, y, z)
	int	*x;
	int	*y;
	int	*z;
{
	*x	= 0.900 * *x;
	*y	= 1.06 * (*y - 14.0);
	*z	= 1.07 * *z;
}
