/*$Id: padimage.c,v 1.5 1995/10/27 16:34:33 ty7777 Exp $*/
/*$Log: padimage.c,v $
 * Revision 1.5  1995/10/27  16:34:33  ty7777
 * Same as the last one.
 *
 * Revision 1.4  1994/11/21  20:26:07  ty7777
 * Edited the comments.
 *
 * Revision 1.3  1994/03/03  20:34:17  ty7777
 * Replace id with header.
 *
 * Revision 1.2  1993/11/03  18:08:55  ty7777
 * Added rcsid.
 *
 * Revision 1.1  1993/03/03  20:38:14  ty7777
 * Initial revision
 **/

/*___________________________________________________________________________
  File:		padimage.c

  Function:	void padimage (float *image3d, short width, short height, short num_slices,
			int nx, int ny, int nz, int first_slice_index, int last_slice_index, 
			int xmargin, int ymargin, float *padimage3d)

  Arguments:	image3d,		3D input image to be padded.
		width,			width of the input image.		
		height,			height of the input image.		
		num_slices,		number of slices for the input image.		
		nx,			x dimension of the padded image.
  		ny,			y dimension of the padded image.
  		nz,			z dimension of the padded image.
		first_slice_index,	slice index in the output image which
					corresponds to the first slice in the input
					image.
		last_slice_index,	slice index in the output image which
					corresponds to the last slice in the input
					image.
		xmargin,		size of margin in the x direction.
		ymargin,		size of margin in the y direction.
		padimage3d,		3D output image, can be used in FORTAN programs 
					as padimage3d [nx][ny][nz]. 

  Description:	This function pads the slices with slice number in [1, first_slice_index - 1]
		with the value from slice first_slice_index and the slices with slice number
		in [last_slice_index + 1, nz] with the value from slice last_slice_index.

  Author:	Tom yang.

  History:	 Created by Tom Yang on 01/04/93. 
___________________________________________________________________________*/

#include <petutil/petutil.h>

static char rcsid [] = "$Header: /home/petsun4/ty7777/src/libnpg/RCS/padimage.c,v 1.5 1995/10/27 16:34:33 ty7777 Exp $";

int padimage (image3d, width, height, num_slices, nx, ny, nz, 
		first_slice_index, last_slice_index, xmargin, ymargin, padimage3d)
	float		*image3d;
	short		width;
	short		height;
	short		num_slices;
	int		nx;
	int		ny;
	int		nz;
	int		first_slice_index;
	int		last_slice_index;
	int		xmargin;
	int		ymargin;
	float		*padimage3d;
{
	int		i;
	int		j;
	int		k;
	int		l;
	int		row_index;
	int		out_row_index;
	int		out_slice_index;
	int		slice_index;

	for (k = 1; k <= nz; k++)
	{
		if (k <= first_slice_index)
			l	= 0;
		else if (k >= last_slice_index)
			l	= num_slices - 1;
		else
			l	= k - first_slice_index;

		slice_index	= l * width * height;
		out_slice_index	= (k - 1) * nx * ny;

		for (j = 0; j < height; j++)
		{
			row_index	= slice_index + j * width;
			out_row_index	= out_slice_index + (j + ymargin) * nx + xmargin;
			for (i = 0; i < width; i++)
			{
				padimage3d [out_row_index + i]	= image3d [row_index + i];
			}
		}
	}

	return SUCCEED;
}
