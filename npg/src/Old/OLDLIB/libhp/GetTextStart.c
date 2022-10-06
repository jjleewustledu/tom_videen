/*$Id: GetTextStart.c,v 1.3 1995/10/30 22:39:01 tom Exp $*/
/*$Log: GetTextStart.c,v $
 * Revision 1.3  1995/10/30  22:39:01  tom
 * change include to remove "petutil"
 *
 * Revision 1.2  1993/12/15  22:59:21  ty7777
 * Working version.
 *
 * Revision 1.1  1993/12/15  22:45:44  ty7777
 * Initial revision
 **/

/*___________________________________________________________________________________
File Name:	GetTextStart.c

Function:	PUBLIC void GetTextStart (char orientation, char alignment, 
		unsigned int width, unsigned int height, int *xstart, int *ystart)

Arguments:	orientation:	orientation of text, either ORIENT_HORT
				or ORIENT_VERT.
 		alignment:	alignment of text, possible values are:	
				ALIGNMENT_LEFT, ALIGNMENT_CENTER, or
				ALIGNMENT_RIGHT.
		width:		width of text block.
		height:		height of text block.
		xstart:		x coordinate of the starting point.
				It need be defined before calling this function.
				Its value is modified after calling of this 
				function.
		ystart:		y coordinate of the starting point.
				It need be defined before calling this function.
				Its value is modified after calling of this 
				function.
				
Return:		None.

Description:	Finds the starting point for a block of text so a 
		bounding rectangle can be defined.

Called by:	view25d and xlabel.		

Calling:			

Author:		Tom Yang 

History:	Created by Tom Yang on 12/15/1993

___________________________________________________________________________________*/

#include <winutil.h>	

static char rcsid [] = "$Header: /home/npggw/tom/src/libhp/RCS/GetTextStart.c,v 1.3 1995/10/30 22:39:01 tom Exp $";

PUBLIC void GetTextStart (orientation, alignment, width, height, xstart, ystart)
	char		orientation;
	char		alignment;
	unsigned int	width;
	unsigned int	height;
	int		*xstart;
	int		*ystart;
{
	if (orientation == ORIENT_HORI)
	{
		if (alignment == ALIGNMENT_CENTER)
		{
			*xstart	-= width / 2;
		}
		else if (alignment == ALIGNMENT_RIGHT)
		{
			*xstart	-= width;
		}
	}
	else if (orientation == ORIENT_VERT)
	{
		if (alignment == ALIGNMENT_CENTER)
		{
			*ystart	-= height / 2;
		}
		else if (alignment == ALIGNMENT_RIGHT)
		{
			*ystart	-= height;
		}
	}
	else
	{
		fprintf (stderr, "Undefined orientation = %d\n", orientation);
	}
}
