/*$Id: drawxray.c,v 1.2 1994/09/02 21:28:18 ty7777 Exp $*/
/*$Log: drawxray.c,v $
 * Revision 1.2  1994/09/02  21:28:18  ty7777
 * Added ac-pc length.
 *
 * Revision 1.1  1993/12/20  21:25:54  ty7777
 * Initial revision
 **/

static char rcsid [] = "$Id: drawxray.c,v 1.2 1994/09/02 21:28:18 ty7777 Exp $";

/*___________________________________________________________________________
  File:		drawxray.c

  Function:	void drawxray (graphics_data)

  Arguments:	graphics_data,	a data structure containing all the components
				for drawing the xray.

  Description:	Draw an xray tracing with the AC-PC line, the Glabella-Inion line,
		the reference slice line, etc.

  History:	Created by Tom Yang on 12/16/93. 
___________________________________________________________________________*/

#include "xray.h"

PUBLIC void drawxray (graphics_data)
	GRAPHICS_DATA	*graphics_data;
{
	DEFINING_POINTS	*defining_points	= graphics_data->defining_points;
	Display		*display		= graphics_data->draw_data->display;
	DRAWING_POINTS	*drawing_points		= graphics_data->drawing_points;
	GC		gc			= graphics_data->draw_data->gc;
	SXR_OUTPUT	*sxr_output		= graphics_data->sxr_output;
	Window		window			= graphics_data->draw_data->window;
	XFontStruct	*font_struct;
	char		stringbuf [MAXLINE];
	int		cols			= graphics_data->cols;
	int		font_height;
	int		i;
	int		j;
	int		rows			= graphics_data->rows;
	int		string_width;
	short int	*xray_tracing		= graphics_data->xray_tracing;

	XClearArea (display, window, 0, 0, DRAW_AREA_WIDTH, DRAW_AREA_HEIGHT, FALSE);

	for (j = 0; j < rows; j++) 
	{
		for (i = 0; i < cols; i++) 
		{
			if (xray_tracing [j * cols + i] > 0)
			{
				XDrawPoint (display, window, gc, 
					DISPLAY_MARGIN_WIDTH + i, 
					DISPLAY_MARGIN_HEIGHT + j);
			}
		}
	}

	XSetForeground (display, gc, graphics_data->xray_color);
	XDrawLine (display, window, gc, 
		drawing_points->glabella.x + DISPLAY_MARGIN_WIDTH,
		drawing_points->glabella.y + DISPLAY_MARGIN_HEIGHT,
		drawing_points->inion.x + DISPLAY_MARGIN_WIDTH,
		drawing_points->inion.y + DISPLAY_MARGIN_HEIGHT);
	XDrawLine (display, window, gc, 
		drawing_points->acpc_anterior.x + DISPLAY_MARGIN_WIDTH,
		drawing_points->acpc_anterior.y + DISPLAY_MARGIN_HEIGHT,
		drawing_points->acpc_posterior.x + DISPLAY_MARGIN_WIDTH,
		drawing_points->acpc_posterior.y + DISPLAY_MARGIN_HEIGHT);
	XDrawLine (display, window, gc, 
		drawing_points->slice_anterior.x + DISPLAY_MARGIN_WIDTH,
		drawing_points->slice_anterior.y + DISPLAY_MARGIN_HEIGHT,
		drawing_points->slice_posterior.x + DISPLAY_MARGIN_WIDTH,
		drawing_points->slice_posterior.y + DISPLAY_MARGIN_HEIGHT);
	XDrawLine (display, window, gc, 
		drawing_points->acpc_center.x + DISPLAY_MARGIN_WIDTH,
		drawing_points->acpc_center.y + DISPLAY_MARGIN_HEIGHT,
		drawing_points->zvertex.x + DISPLAY_MARGIN_WIDTH,
		drawing_points->zvertex.y + DISPLAY_MARGIN_HEIGHT);
	XDrawLine (display, window, gc, 
		drawing_points->acpc_center.x + DISPLAY_MARGIN_WIDTH,
		drawing_points->acpc_center.y + DISPLAY_MARGIN_HEIGHT,
		drawing_points->ref_normalref.x + DISPLAY_MARGIN_WIDTH,
		drawing_points->ref_normalref.y + DISPLAY_MARGIN_HEIGHT);

	XSetForeground (display, gc, graphics_data->marker_color);
	DrawSymbol (display, window, gc, 
		drawing_points->glabella.x + DISPLAY_MARGIN_WIDTH, 
		drawing_points->glabella.y + DISPLAY_MARGIN_HEIGHT, 
		CROSSHAIR_SIZE, CROSSHAIR_SIZE, CROSSHAIR);
	DrawSymbol (display, window, gc, 
		drawing_points->inion.x + DISPLAY_MARGIN_WIDTH, 
		drawing_points->inion.y + DISPLAY_MARGIN_HEIGHT, 
		CROSSHAIR_SIZE, CROSSHAIR_SIZE, CROSSHAIR);

	DrawSymbol (display, window, gc, 
		drawing_points->acpc_anterior.x + DISPLAY_MARGIN_WIDTH, 
		drawing_points->acpc_anterior.y + DISPLAY_MARGIN_HEIGHT, 
		CROSSHAIR_SIZE, CROSSHAIR_SIZE, CROSSHAIR);
	DrawSymbol (display, window, gc, 
		drawing_points->acpc_posterior.x + DISPLAY_MARGIN_WIDTH, 
		drawing_points->acpc_posterior.y + DISPLAY_MARGIN_HEIGHT, 
		CROSSHAIR_SIZE, CROSSHAIR_SIZE, CROSSHAIR);
	DrawSymbol (display, window, gc, 
		drawing_points->slice_anterior.x + DISPLAY_MARGIN_WIDTH, 
		drawing_points->slice_anterior.y + DISPLAY_MARGIN_HEIGHT, 
		CROSSHAIR_SIZE, CROSSHAIR_SIZE, CROSSHAIR);
	DrawSymbol (display, window, gc, 
		drawing_points->slice_posterior.x + DISPLAY_MARGIN_WIDTH, 
		drawing_points->slice_posterior.y + DISPLAY_MARGIN_HEIGHT, 
		CROSSHAIR_SIZE, CROSSHAIR_SIZE, CROSSHAIR);
	DrawSymbol (display, window, gc, 
		(drawing_points->slice_anterior.x 
		+ drawing_points->slice_posterior.x) / 2 + DISPLAY_MARGIN_WIDTH, 
		(drawing_points->slice_anterior.y +
		drawing_points->slice_posterior.y) / 2 + DISPLAY_MARGIN_HEIGHT, 
		CROSSHAIR_SIZE, CROSSHAIR_SIZE, CROSSHAIR);
	DrawSymbol (display, window, gc, 
		drawing_points->zvertex.x + DISPLAY_MARGIN_WIDTH, 
		drawing_points->zvertex.y + DISPLAY_MARGIN_HEIGHT, 
		CROSSHAIR_SIZE, CROSSHAIR_SIZE, CROSSHAIR);

	XSetForeground (display, gc, graphics_data->pet_color);
		DrawSymbol (display, window, gc, 
		drawing_points->vertex_zmax.x + DISPLAY_MARGIN_WIDTH, 
		drawing_points->vertex_zmax.y + DISPLAY_MARGIN_HEIGHT, 
		CROSSHAIR_SIZE, CROSSHAIR_SIZE, CROSSHAIR);

	XSetForeground (display, gc, graphics_data->xray_color);

	/*
	for (i = 0; i < sxr_output->number_slices; i++)
	{
		if (sxr_output->rlslicedim [i - 1] >= 1.0)
		{
			XDrawLine (display, window, gc, 
			drawing_points->slices_ap [2 * i].x + DISPLAY_MARGIN_WIDTH,
			drawing_points->slices_ap [2 * i].y + DISPLAY_MARGIN_HEIGHT,
			drawing_points->slices_ap [2 * i + 1].x + DISPLAY_MARGIN_WIDTH,
			drawing_points->slices_ap [2 * i + 1].y + DISPLAY_MARGIN_HEIGHT);
		}
	}
	*/

	/* 
	 * Write the AC-PC length in mm and the reference slice number.
	 */

	/* 
	 * Load text font. 
	 */
	font_struct	= XLoadQueryFont (display, ACPC_FONT);
	if (font_struct == 0)
		fprintf (stderr, "Can Not Find Font %s\n", ACPC_FONT);
	else
		XSetFont (display, gc, font_struct->fid);

	sprintf (stringbuf, "Length of AC-PC = %.1f (mm)     Reference Slice = %d", 
		sxr_output->apxray * MMS_PER_CM, sxr_output->reference_slice);
	font_height	= font_struct->ascent + font_struct->descent;
	string_width	= XTextWidth (font_struct, stringbuf, strlen (stringbuf));

	XDrawString (display, window, gc, (MAIN_WINDOW_WIDTH - string_width) / 2, 
		2 * font_height, stringbuf, strlen (stringbuf));

	XFreeFont (display, font_struct);
}
