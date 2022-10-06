/*$Id: zoom.c,v 1.2 1995/10/31 14:54:42 tom Exp $*/
/*$Log: zoom.c,v $
 * Revision 1.2  1995/10/31  14:54:42  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.1  1995/10/10  15:49:09  ty7777
 * Initial revision
 **/

#include <petutil.h>
#include <stdlib.h>
#include <math.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libpetf77/RCS/zoom.c,v 1.2 1995/10/31 14:54:42 tom Exp $";

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

PUBLIC void zoom (oimage, owidth, oheight, xzoom, yzoom, nwidth, nheight, nimage)
	short		*oimage;	
	int		owidth;
	int		oheight;
	FLOATPARAMETER	xzoom;
	FLOATPARAMETER	yzoom;
	int		nwidth; 
	int		nheight;
	short		*nimage;	/* Output image */	
{
	float		xscale;
	float		yscale;
	unsigned int	*xindex, *yindex;
	unsigned int	i, j;
	unsigned int	row_index;

	xscale	= FLOATPARAMETERVALUE (xzoom);
	yscale	= FLOATPARAMETERVALUE (yzoom);
	xindex	= BuildIndex (owidth, xscale, nwidth);
	yindex	= BuildIndex (oheight, yscale, nheight);

	for (j = 0; j < nheight; j++) 
	{
		row_index	= *(yindex + j) * owidth;
		for (i = 0; i < nwidth; i++) 
		{
			*nimage	= oimage [row_index + *(xindex + i)];
			nimage++;
		}
	}

	free (xindex);
	free (yindex);
}
