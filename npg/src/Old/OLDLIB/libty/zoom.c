/*$Id: zoom.c,v 1.3 1995/11/10 16:29:55 tom Exp $*/
/*$Log: zoom.c,v $
 * Revision 1.3  1995/11/10  16:29:55  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.2  1994/03/03  20:47:28  ty7777
 * Replace id with header.
 *
 * Revision 1.1  1993/11/11  17:31:04  ty7777
 * Initial revision
 **/

#include <petutil.h>
#include <stdlib.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libnpg/RCS/zoom.c,v 1.3 1995/11/10 16:29:55 tom Exp $";

static unsigned int *BuildIndex (width, zoom, rwidth)
	unsigned int	width;
	float		zoom;
	unsigned int	rwidth;
{ 
	unsigned int	i;
	unsigned int	*index;
	unsigned int	temp_index;

	index	= (unsigned int *) malloc (sizeof (unsigned int) * rwidth);

	for (i = 0; i < rwidth; i++)
	{
		temp_index	= ((float) i) / zoom;
		if (temp_index < width)
			*(index + i)	= temp_index;
		else
		{
			*(index + i)	= width - 1;
		}
	}

	return (index);
}

PUBLIC void zoom (oimage, owidth, oheight, xzoom, yzoom, xwidth, ywidth, nimage)
	short	*oimage;	/* Input image to be interpolated or expanded */	
	int	owidth;
	int	oheight;
	float	xzoom;
	float	yzoom;
	int	xwidth; 
	int	ywidth;
	short	*nimage;	/* Output image */	
{
	unsigned int	*xindex, *yindex;
	unsigned int	i, j;
	unsigned int	row_index;

	xindex	= BuildIndex (owidth, xzoom, xwidth);
	yindex	= BuildIndex (oheight, yzoom, ywidth);

	for (j = 0; j < ywidth; j++) 
	{
		row_index	= *(yindex + j) * owidth;
		for (i = 0; i < xwidth; i++) 
		{
			*nimage	= oimage [row_index + *(xindex + i)];
			nimage++;
		}
	}

	free (xindex);
	free (yindex);
}
