/*$Id: z2slice.c,v 1.1 1996/07/10 20:54:22 yang Exp $*/
/*$Log: z2slice.c,v $
 * Revision 1.1  1996/07/10  20:54:22  yang
 * Initial revision
 **/

/*___________________________________________________________________________
  File:		z2slice.c

  Function:	PUBLIC int z2slice (float z, float zvoxsize, int zorigin)

  Arguments:	z,		z value corresponding to slice.
		zvoxsize,	voxel size in the z direction.
		zorigin,	slice number corresponding to the origin
				of the z axis.

  Return:	The slice number.

  Description:	This function converts a z value to a slice number.

  Called by:	portal.c for program view25d.	

  Calling:	None.	

  History:	Created by Tom Yang on 04/14/94. 
___________________________________________________________________________*/

#include <petutil/petutil.h>

static char rcsid [] = "$Header: /home/petsun4/yang/src/libnpg/RCS/z2slice.c,v 1.1 1996/07/10 20:54:22 yang Exp $";

PUBLIC int z2slice (z, zvoxsize, zorigin)
	float	z;
	float	zvoxsize;
	int	zorigin;
{
	int	slice;

	slice	= ROUND (zorigin - z / zvoxsize);
	return slice;
}

PUBLIC int x2slice (x, xvoxsize, xorigin)
	float	x;
	float	xvoxsize;
	int	xorigin;
{
	int	slice;

	slice	= ROUND (xorigin + x / xvoxsize);

	return slice;
}

PUBLIC int y2slice (y, yvoxsize, yorigin)
	float	y;
	float	yvoxsize;
	int	yorigin;
{
	int	slice;

	slice	= ROUND (yorigin - y / yvoxsize);

	return slice;
}
