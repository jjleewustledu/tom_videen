/*$Id: slice2z.c,v 1.2 1995/10/30 22:39:01 tom Exp $*/
/*$Log: slice2z.c,v $
 * Revision 1.2  1995/10/30  22:39:01  tom
 * change include to remove "petutil"
 *
 * Revision 1.1  1994/04/14  20:04:20  ty7777
 * Initial revision
 **/

/*___________________________________________________________________________
  File:		slice2z.c

  Function:	PUBLIC float slice2z (int slice, int zorigin)

  Arguments:	slice,		slice number to convert.
		zorigin,	slice number corresponding to the origin
				of the z axis.

  Return:	The z value in mm.

  Description:	This function converts a slice number to a z value.

  Called by:	portal.c for program view25d.	

  Calling:	None.	

  History:	Created by Tom Yang on 04/14/94. 
___________________________________________________________________________*/

#include <petutil.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libhp/RCS/slice2z.c,v 1.2 1995/10/30 22:39:01 tom Exp $";

PUBLIC float slice2z (slice, zorigin)
	int	slice;
	int	zorigin;
{
	float	z;

	z	= (zorigin - slice) * 2.0;

	return z;
}
