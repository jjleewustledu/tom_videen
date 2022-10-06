/*$Id: trim.c,v 1.5 1995/11/10 16:29:31 tom Exp $*/
/*$Log: trim.c,v $
 * Revision 1.5  1995/11/10  16:29:31  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.4  1994/11/21  20:34:48  ty7777
 * Edited the comments.
 *
 * Revision 1.3  1994/03/03  20:44:54  ty7777
 * Replace id with header.
 *
 * Revision 1.2  1993/11/03  18:11:35  ty7777
 * Added rcsid.
 *
 * Revision 1.1  1993/03/03  20:39:58  ty7777
 * Initial revision
 **/

/*___________________________________________________________________________
  File:		trim.c

  Function:	void trim (float *padimage3d, int nx, int ny, int nz, short width,
		short height, short num_slices, int first_slice_index, int last_slice_index, 
		int xmargin, int ymargin, float *image3d)

  Arguments:	padimage3d,	padded input image.
		nx,		x dimension of the input image.
  		ny,		y dimension of the input image.
  		nz,		z dimension of the input image.
		width,		width of the trimmed image.
		height,		height of the trimmed image.
		num_slices,	number of slices for the trimmed image.
		xmargin,	margin size in pixels in the x direction.
		ymargin,	margin size in pixels in the y direction.
		image3d,	3D output trimmed image, can be used in FORTAN programs as 
				image3d [nx][ny][nz]. 

  Description:	This function trims the slices with slice number from first_slice_index to
  		last_slice_index.

  Author:	Tom Yang.
  History:	Created by Tom Yang on 01/05/93. 
___________________________________________________________________________*/

#include <petutil.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libnpg/RCS/trim.c,v 1.5 1995/11/10 16:29:31 tom Exp $";

int trim (padimage3d, nx, ny, nz, width, height, num_slices, 
		first_slice_index, last_slice_index, xmargin, ymargin, image3d)
	float		*padimage3d;
	int		nx;
	int		ny;
	int		nz;
	short		width;
	short		height;
	short		num_slices;
	int		first_slice_index;
	int		last_slice_index;
	int		xmargin;
	int		ymargin;
	float		*image3d;
{
	int		i;
	int		j;
	int		k;
	int		row_index;
	int		pad_row_index;
	int		pad_slice_index;
	int		slice_index;

	for (k = first_slice_index; k <= last_slice_index; k++)
	{
		slice_index	= (k - first_slice_index) * width * height;
		pad_slice_index	= (k - 1) * nx * ny;

		for (j = 0; j < height; j++)
		{
			row_index	= slice_index + j * width;
			pad_row_index	= pad_slice_index + (j + ymargin) * nx + xmargin;
			for (i = 0; i < width; i++)
			{
				image3d [row_index + i]	= padimage3d [pad_row_index + i];
			}
		}
	}

	return SUCCEED;
}
