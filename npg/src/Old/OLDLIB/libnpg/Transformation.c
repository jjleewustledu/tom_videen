/*$Id: Transformation.c,v 1.5 1995/11/10 16:27:54 tom Exp $*/
/*$Log: Transformation.c,v $
 * Revision 1.5  1995/11/10  16:27:54  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.4  1995/06/02  15:06:56  ty7777
 * Changed dScaleSize to dScaleWidth and dScaleHeight.
 *
 * Revision 1.3  1994/11/09  17:38:09  ty7777
 * Edited the comments.
 *
 * Revision 1.2  1994/03/03  19:58:22  ty7777
 * Replace id with header.
 *
 * Revision 1.1  1993/09/10  20:35:49  ty7777
 * Initial revision
 **/

#include <petutil.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libnpg/RCS/Transformation.c,v 1.5 1995/11/10 16:27:54 tom Exp $";

/*_____________________________________________________________________________      
	Function:       void Transformation (float dScaleWidth, float dOldCenterx,
			float dOldCentery, int nOldx, int nOldy, int nNewCEnterx,
			int nNewCentery, int *npNewx, int *npNewy)

	Arguments:	dScaleWidth:		scale factor for x.	
			dScaleHeight:		scale factor for y.	
			dOldWidth, dOldHeight:	old space dimension.
			nNewWidth, nNewHeight:	new space dimension.
			nOldx, nOldy:		old coordinates.
			npNewx, npNewy:		new coordinates, returned.

	Description:	Transform coordinates from one space to another.

	Return Value:	none.

	Author:		Tom Yang 
			(03/18/1991)
______________________________________________________________________________*/ 

PUBLIC void Transformation (dScaleWidth, dScaleHeight, dOldCenterx, dOldCentery, 
			     nOldx, nOldy, nNewCenterx, nNewCentery, npNewx, npNewy)
float	dScaleWidth;
float	dScaleHeight;
float	dOldCenterx, dOldCentery;
int	nOldx, nOldy;
int	nNewCenterx, nNewCentery;
int	*npNewx, *npNewy;
{
	*npNewx	= ROUND (dScaleWidth * (nOldx - dOldCenterx)) + nNewCenterx;
	*npNewy = ROUND (dScaleHeight * (nOldy - dOldCentery)) + nNewCentery;
}
