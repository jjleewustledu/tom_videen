/*$Id: reflection.c,v 1.4 1995/11/10 16:29:22 tom Exp $*/
/*$Log: reflection.c,v $
 * Revision 1.4  1995/11/10  16:29:22  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.3  1994/03/03  20:42:44  ty7777
 * Replace id with header.
 *
 * Revision 1.2  1993/10/14  19:43:02  ty7777
 * Same as before.
 *
 * Revision 1.1  1993/10/04  20:47:17  ty7777
 * Initial revision
 **/

/*___________________________________________________________________________
  File:		reflection.c

  Function:	int reflection (int width, int height, short * slice_data)

  Arguments:	width:		width of slice.
		height:		height of slice.
		slice_data:	slice to be reflected.

  Return:	SUCCEED: successful; FAIL: error.

  Description:	This function reflects the slice defined by slice_data 
		at the midline of the horizontal direction. 

  Called by:	getimage () and getrealimg () in libhp.a.	

  Calling:	

  Author:	Tom Yang.

  History:	Created by Tom Yang on 10/03/93. 
___________________________________________________________________________*/

#include <petutil.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libnpg/RCS/reflection.c,v 1.4 1995/11/10 16:29:22 tom Exp $";

PUBLIC int reflection (width, height, slice_data)
	int	width;
	int	height;
	short	*slice_data;
{
	int	i;
	int	j;
	int	midline;
	int	rows;
	short	temp;

	midline	= width / 2;
	for (j = 0; j < height; j++)
	{
		rows	= j * width;
		for (i = 0; i < midline; i++)
		{
			temp					= slice_data [rows + i];
			slice_data [rows + i]			= slice_data [rows + (width - 1) - i];
			slice_data [rows + (width - 1) - i]	= temp;
		}
	}

	return SUCCEED;
}
