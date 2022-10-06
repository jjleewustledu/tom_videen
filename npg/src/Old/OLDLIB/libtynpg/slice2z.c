/*$Id: slice2z.c,v 1.1 1996/07/10 20:53:38 yang Exp $*/
/*$Log: slice2z.c,v $
 * Revision 1.1  1996/07/10  20:53:38  yang
 * Initial revision
 **/

/*___________________________________________________________________________
  File:		slice2z.c

  Function:	PUBLIC float slice2z (int slice, float zvoxsize, int zorigin)

  Arguments:	slice,		slice number to convert.
		zvoxsize,	voxel size in the z direction.
		zorigin,	slice number corresponding to the origin
				of the z axis.

  Return:	The z value in mm.

  Description:	This function converts a slice number to a z value.

  Called by:	portal.c for program view25d.	

  Calling:	None.	

  History:	Created by Tom Yang on 04/14/94. 
___________________________________________________________________________*/

#include <petutil/petutil.h>

static char rcsid [] = "$Header: /home/petsun4/yang/src/libnpg/RCS/slice2z.c,v 1.1 1996/07/10 20:53:38 yang Exp $";

PUBLIC float slice2z (slice, zvoxsize, zorigin)
	int	slice;
	float	zvoxsize;
	int	zorigin;
{
	float	z;

	z	= (zorigin - slice) * zvoxsize;

	return z;
}

PUBLIC float slice2x (slice, xvoxsize, xorigin)
	int	slice;
	float	xvoxsize;
	int	xorigin;
{
	float	x;

	x	= (slice - xorigin) * xvoxsize;

	return x;
}

PUBLIC float slice2y (slice, yvoxsize, yorigin)
	int	slice;
	float	yvoxsize;
	int	yorigin;
{
	float	y;

	y	= (yorigin - slice) * yvoxsize;

	return y;
}
