/*$Id: xor.c,v 1.2 1993/11/03 18:13:40 ty7777 Exp $*/
/*$Log: xor.c,v $
 * Revision 1.2  1993/11/03  18:13:40  ty7777
 * Added rcsid.c
 *
 * Revision 1.1  1993/06/11  18:48:42  ty7777
 * Initial revision
 **/

/**************************************************************************
  * CreateXorGC ():  utility routine  
  *
  *  Copyright 1989 by Prentice Hall
  *  All Rights Reserved
  *
  *************************************************************************/
#include <petutil/petutil.h>

static char rcsid [] = "$Header: /home/petsun4/ty7777/src/libhp/RCS/xor.c,v 1.2 1993/11/03 18:13:40 ty7777 Exp $";

PUBLIC GC CreateXorGC (w)
	Widget	w;
{
	Arg		wargs [2];
	GC		gc;
	XGCValues	values;

	/*
	 * Get the colors used by the widget.
	 */
	XtSetArg (wargs [0], XtNforeground, &values.foreground);
	XtSetArg (wargs [1], XtNbackground, &values.background);
	XtGetValues (w, wargs, 2);

	/*
	 * Set the fg to the XOR of the fg and bg, so if it is
	 * XOR'ed with bg, the result will be fg and vice-versa.
	 * This effectively achieves inverse video for the line.
	 */
	values.foreground	= values.foreground ^ values.background;

	/*
	 * Set the rubber band gc to use XOR mode and draw 
	 * a dashed line.
	 */
	values.line_style	= LineSolid;
	values.function		= GXxor;

	gc	= XtGetGC (w, GCForeground | GCBackground | GCFunction 
			| GCLineStyle, &values);

	return gc;
}
