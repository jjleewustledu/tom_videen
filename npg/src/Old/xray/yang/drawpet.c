/*$Id: drawpet.c,v 1.2 1996/07/10 20:55:13 yang Exp $*/
/*$Log: drawpet.c,v $
 * Revision 1.2  1996/07/10  20:55:13  yang
 * Changed the xinc and yinc for ECAT_961.
 *
 * Revision 1.1  1996/07/10  14:52:31  yang
 * Initial revision
 **/

static char rcsid [] = "$Id: drawpet.c,v 1.2 1996/07/10 20:55:13 yang Exp $";

/*___________________________________________________________________________
  File:		drawpet.c

  Function:	void drawpet (display_pet_data)

  Arguments:	display_pet_data,	a data structure containing all the 
					components for drawing the pet image.

  Description:	Draw the PET image on screen in rows and columns.

  History:	Created by Tom Yang on 09/19/94. 
___________________________________________________________________________*/

#include "xray.h"

PUBLIC void drawpet (display_pet_data)
	DISPLAY_PET_DATA	*display_pet_data;
{
	SXR_OUTPUT	*sxr_output	= display_pet_data->sxr_output;
	llist		petlist		= display_pet_data->petlist;
	DRAW_DATA	*draw_data	= display_pet_data->draw_data;
	Display		*display	= draw_data->display;
	Window		window		= draw_data->window;
	GC		gc		= draw_data->gc;
	PET_SLICE_DATA	*slice;
	POINT2D		bottom;
	POINT2D		left;
	POINT2D		right;
	POINT2D		top;
	XFontStruct	*font_struct;
	XPoint		points [5];
	char		stringbuf [MAXLINE];
	float		scale;
	int		cx;
	int		cy;
	int		font_height;
	int		height;
	int		i;
	int		j;
	int		list_size;
	int		n;
	int		pettnum		= display_pet_data->pettnum;
	int		string_width;
	int		width;
	int		xinc;
	int		xoffset;
	int		xstart;
	int		yinc;
	int		yoffset;
	int		ystart;
	short		*databuf;

	XClearArea (display, window, 0, 0, DRAW_AREA_WIDTH, DRAW_AREA_HEIGHT, FALSE);
	list_size	= llsize (petlist);

	if (pettnum == ECAT_SCANNER)
	{
		scale	= ECAT_SCALE;
		xinc	= ECAT_XINC;	
		yinc	= ECAT_YINC;	
		xstart	= ECAT_XSTART;
		ystart	= ECAT_YSTART;
	}
	else if (pettnum == ECAT961_SCANNER)
	{
		scale	= ECAT961_SCALE;
		xinc	= ECAT961_XINC;	
		yinc	= ECAT961_YINC;	
		xstart	= ECAT961_XSTART;
		ystart	= ECAT961_YSTART;
	}
	else
	{
		scale	= PETT6_SCALE;
		xinc	= PETT6_XINC;	
		yinc	= PETT6_YINC;	
		xstart	= PETT6_XSTART;
		ystart	= PETT6_YSTART;
	}

	/* 
	 * Load text font. 
	 */
	font_struct	= XLoadQueryFont (display, PET_LABEL_FONT);
	if (font_struct == 0)
		fprintf (stderr, "Can Not Find Font %s\n", PET_LABEL_FONT);
	else
		XSetFont (display, gc, font_struct->fid);

	llhead (petlist);
	for (n = 1; n <= list_size; n++)
	{
		/*
		 * Desired center of slice.
		 */
		if (pettnum != ECAT961_SCANNER)
		{
			cx	= xstart + ((n - 1) % 7) * xinc;
			cy	= ystart + ((n - 1) / 7) * yinc; 
		}
		else
		{
			cx	= xstart + ((n - 1) % 9) * xinc;
			cy	= ystart + ((n - 1) / 9) * yinc; 
		}

		slice = llretrieve (petlist);

		/* 
		 * Compute pet image offset. 
		 */
		Transformation (scale, scale, CX_IMAGE, CY_IMAGE, slice->xstart, slice->ystart,
				cx, cy, &xoffset, &yoffset);
		Transformation (scale, scale, CX_IMAGE, CY_IMAGE, slice->bottom.x, slice->bottom.y,
				cx, cy, &bottom.x, &bottom.y);
		Transformation (scale, scale, CX_IMAGE, CY_IMAGE, slice->left.x, slice->left.y,
				cx, cy, &left.x, &left.y);
		Transformation (scale, scale, CX_IMAGE, CY_IMAGE, slice->right.x, slice->right.y,
				cx, cy, &right.x, &right.y);
		Transformation (scale, scale, CX_IMAGE, CY_IMAGE, slice->top.x, slice->top.y,
				cx, cy, &top.x, &top.y);
		/* 
		 * Resize pet image. 
		 */
		if (left.x < right.x && top.y < bottom.y)
		{
			if (slice->slice_index == sxr_output->peakslice)
				XSetForeground (display, gc, display_pet_data->peak_color);
			else
				XSetForeground (display, gc, display_pet_data->pet_color);
			width	= ROUND (slice->width * scale);
			height	= ROUND (slice->height * scale);
			databuf	= (short *) pkg_malloc (width * height * sizeof (short), 
						"DisplayPetCalback", "databuf"); 
			interp (slice->input, slice->width, slice->height, databuf, width, height);

			for (j = 0; j < height; j++) 
			{
				for (i = 0; i < width; i++) 
				{
					if (databuf [j * width + i] > 0)
					{
						XDrawPoint (display, window, gc, 
						xoffset + i, yoffset + j);
					}
				}
			}
			free (databuf);

			/*
		 	 * Draw bounding rectangle lines.
		 	 */
			XSetForeground (display, gc, display_pet_data->xray_color);

			points [0].x	= left.x;
			points [0].y	= top.y;

			points [1].x	= left.x;
			points [1].y	= bottom.y;

			points [2].x	= right.x;
			points [2].y	= bottom.y;

			points [3].x	= right.x;
			points [3].y	= top.y;

			points [4].x	= left.x;
			points [4].y	= top.y;

			XDrawLines (display, window, gc, points, 5, CoordModeOrigin);
		}
		else
		{
			left.x		= cx;
			right.x		= cx;
			bottom.y	= cy;
		}

		sprintf (stringbuf, "%.0f     %d", slice->slice_peak, slice->slice_index);
		font_height	= font_struct->ascent + font_struct->descent;
		string_width	= XTextWidth (font_struct, stringbuf, strlen (stringbuf));
		XDrawString (display, window, gc, right.x - string_width, 
			bottom.y + font_height, stringbuf, strlen (stringbuf));

		llnext (petlist);
	}

	XFreeFont (display, font_struct);
}
