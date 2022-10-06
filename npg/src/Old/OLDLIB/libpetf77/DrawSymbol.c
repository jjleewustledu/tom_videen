/*$Id: DrawSymbol.c,v 1.2 1995/10/31 14:54:07 tom Exp $*/
/*$Log: DrawSymbol.c,v $
 * Revision 1.2  1995/10/31  14:54:07  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.1  1995/10/09  17:13:23  ty7777
 * Initial revision
 **/

/*____________________________________________________________________	
	Function::
		DrawSymbol	
	Parameters:
		display:	X Display pointer.
		window:		X window to draw into.
		gc:		graphics context.
		x:		upper left x coord. of rectangle.
		y:		upper left y coord. of rectangle.
		w:		width of rectangle in pixels.
		h:		height of rectangle in pixels.
	Description:
		Draw AF3D symbols.
	Author:	Tom (Tongzeng) Yang
		(08/28/1991)
____________________________________________________________________*/

#include <X11/StringDefs.h>
#include <X11/Intrinsic.h> 
#include <petutil.h>
#include <winutil.h>

void DrawSymbol (display, window, gc, x, y, w, h, symbol_type)
Display		*display;
Window		window;
GC		gc;
short		x, y;			/* coord. of upper left cornet of rect. */
unsigned short	w, h;			/* width and height of rectangle */
char		symbol_type;
{
	XPoint	points [5];

	switch (symbol_type)
	{
		case RECTANGLE:
			XDrawRectangle (display, window, gc, x, y, w, h); 
			break;
		case FILLED_RECTANGLE:
			XFillRectangle (display, window, gc, x, y, w, h); 
			break;
		case CIRCLE:
			XDrawArc (display, window, gc, x, y, w, h, 0, 360 * 64); 
			break;
		case FILLED_CIRCLE:
			XFillArc (display, window, gc, x, y, w, h, 0, 360 * 64); 
			break;
		case DIAMOND:
		case FILLED_DIAMOND:
			points [0].x	= x + w / 2;
			points [0].y	= y;
			points [1].x	= x + w;
			points [1].y	= y + h / 2;
			points [2].x	= x + w / 2;
			points [2].y	= y + h;
			points [3].x	= x;
			points [3].y	= y + h / 2;
			points [4].x	= points [0].x;
			points [4].y	= points [0].y;

			if (symbol_type == DIAMOND)
			{
				XDrawLines (display, window, gc, 
					points, 5, CoordModeOrigin);
			}
			else if (symbol_type == FILLED_DIAMOND)
			{
				XFillPolygon (display, window, gc, 
					points, 5, Convex, CoordModeOrigin);
			}
			break;
		case UP_TRIANGLE:
		case FILLED_UP_TRIANGLE:
			points [0].x	= x + w / 2;
			points [0].y	= y;
			points [1].x	= x;
			points [1].y	= y + h;
			points [2].x	= x + w;
			points [2].y	= y + h;
			points [3].x	= points [0].x;
			points [3].y	= points [0].y;

			if (symbol_type == UP_TRIANGLE)
			{
				XDrawLines (display, window, gc, 
					points, 4, CoordModeOrigin);
			}
			else if (symbol_type == FILLED_UP_TRIANGLE)
			{
				XFillPolygon (display, window, gc, 
					points, 4, Convex, CoordModeOrigin);
			}
			break;
		case DOWN_TRIANGLE:
		case FILLED_DOWN_TRIANGLE:
			points [0].x	= x + w / 2;
			points [0].y	= y + h;
			points [1].x	= x;
			points [1].y	= y;
			points [2].x	= x + w;
			points [2].y	= y;
			points [3].x	= points [0].x;
			points [3].y	= points [0].y;

			if (symbol_type == DOWN_TRIANGLE)
			{
				XDrawLines (display, window, gc, 
					points, 4, CoordModeOrigin);
			}
			else if (symbol_type == FILLED_DOWN_TRIANGLE)
			{
				XFillPolygon (display, window, gc, 
					points, 4, Convex, CoordModeOrigin);
			}
			break;
		case LEFT_TRIANGLE:
		case FILLED_LEFT_TRIANGLE:
			points [0].x	= x;
			points [0].y	= y + h / 2;
			points [1].x	= x + w;
			points [1].y	= y;
			points [2].x	= x + w;
			points [2].y	= y + h;
			points [3].x	= points [0].x;
			points [3].y	= points [0].y;

			if (symbol_type == LEFT_TRIANGLE)
			{
				XDrawLines (display, window, gc, 
					points, 4, CoordModeOrigin);
			}
			else if (symbol_type == FILLED_LEFT_TRIANGLE)
			{
				XFillPolygon (display, window, gc, 
					points, 4, Convex, CoordModeOrigin);
			}
			break;
		case RIGHT_TRIANGLE:
		case FILLED_RIGHT_TRIANGLE:
			points [0].x	= x + w;
			points [0].y	= y + h / 2;
			points [1].x	= x;
			points [1].y	= y;
			points [2].x	= x;
			points [2].y	= y + h;
			points [3].x	= points [0].x;
			points [3].y	= points [0].y;

			if (symbol_type == RIGHT_TRIANGLE)
			{
				XDrawLines (display, window, gc, 
					points, 4, CoordModeOrigin);
			}
			else if (symbol_type == FILLED_RIGHT_TRIANGLE)
			{
				XFillPolygon (display, window, gc, 
					points, 4, Convex, CoordModeOrigin);
			}
			break;
		case CROSSHAIR:
				XDrawLine (display, window, gc, x - w / 2, y, x + w / 2, y);
				XDrawLine (display, window, gc, x, y - h / 2, x, y + h / 2);
		default: 
			break;
	}
}
