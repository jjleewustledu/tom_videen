/*$Id: z2slice.c,v 1.2 1995/11/10 16:29:53 tom Exp $*/
/*$Log: z2slice.c,v $
 * Revision 1.2  1995/11/10  16:29:53  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.1  1994/04/14  20:04:01  ty7777
 * Initial revision
 **/

/*___________________________________________________________________________
  File:		z2slice.c

  Function:	PUBLIC int z2slice (float z, int zorigin)

  Arguments:	z,		z value corresponding to slice.
		zorigin,	slice number corresponding to the origin
				of the z axis.

  Return:	The slice number.

  Description:	This function converts a z value to a slice number.

  Called by:	portal.c for program view25d.	

  Calling:	None.	

  History:	Created by Tom Yang on 04/14/94. 
___________________________________________________________________________*/

#include <petutil.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libnpg/RCS/z2slice.c,v 1.2 1995/11/10 16:29:53 tom Exp $";

PUBLIC int z2slice (z, zorigin)
	float	z;
	int	zorigin;
{
	int	slice;

	slice	= ROUND (zorigin - z / 2.0);

	return slice;
}
