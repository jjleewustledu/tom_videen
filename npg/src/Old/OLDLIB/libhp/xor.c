/*$Id: xor.c,v 1.5 1995/10/30 22:39:01 tom Exp $*/
/*$Log: xor.c,v $
 * Revision 1.5  1995/10/30  22:39:01  tom
 * change include to remove "petutil"
 *
 * Revision 1.4  1994/10/27  19:48:46  ty7777
 * Use winutil.h
 *
 * Revision 1.3  1994/09/20  15:29:02  ty7777
 * Same as the last version.
 *
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
#include <winutil.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libhp/RCS/xor.c,v 1.5 1995/10/30 22:39:01 tom Exp $";

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
