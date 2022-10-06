/*$Id: pix_buttons.c,v 1.3 1993/11/03 18:10:47 ty7777 Exp $*/
/*$Log: pix_buttons.c,v $
 * Revision 1.3  1993/11/03  18:10:47  ty7777
 * Added rcsid.
 *
 * Revision 1.2  1993/07/06  18:13:04  ty7777
 * Same as the last one.
 *
 * Revision 1.1  1993/03/03  20:39:31  ty7777
 * Initial revision
 **/


 /*________________________________________________
  *    pixmap_browser functions
  *         From:
  *                   The X Window System, 
  *            Programming and Applications with Xt
  *                   OSF/Motif Edition
  *         by
  *                Douglas Young
  *              Prentice Hall, 1990
  *
  *                 Example described on pages:  200-202
  *
  *
  *  Copyright 1989 by Prentice Hall
  *  All Rights Reserved
  *
  * This code is based on the OSF/Motif widget set and the X Window System
  *
  * Permission to use, copy, modify, and distribute this software for 
  * any purpose and without fee is hereby granted, provided that the above
  * copyright notice appear in all copies and that both the copyright notice
  * and this permission notice appear in supporting documentation.
  *
  * Prentice Hall and the author disclaim all warranties with regard to 
  * this software, including all implied warranties of merchantability and fitness.
  * In no event shall Prentice Hall or the author be liable for any special,
  * indirect or cosequential damages or any damages whatsoever resulting from 
  * loss of use, data or profits, whether in an action of contract, negligence 
  * or other tortious action, arising out of or in connection with the use 
  * or performance of this software.
  *
  * Open Software Foundation is a trademark of The Open Software Foundation, Inc.
  * OSF is a trademark of Open Software Foundation, Inc.
  * OSF/Motif is a trademark of Open Software Foundation, Inc.
  * Motif is a trademark of Open Software Foundation, Inc.
  * DEC is a registered trademark of Digital Equipment Corporation
  * HP is a registered trademark of the Hewlett Packard Company
  * DIGITAL is a registered trademark of Digital Equipment Corporation
  * X Window System is a trademark of the Massachusetts Institute of Technology
  __________________________________________________________________________________*/

#include <X11/StringDefs.h>
#include <X11/Intrinsic.h> 
#include <Xm/Xm.h>

static char rcsid [] = "$Header: /home/petsun4/ty7777/src/libnpg/RCS/pix_buttons.c,v 1.3 1993/11/03 18:10:47 ty7777 Exp $";

void xs_register_pattern (w, name, bits, width, height)
	Widget         w;
	char          *name;
	unsigned char *bits;
	int            width, height;
{
	XImage *image;

	image	= XCreateImage (XtDisplay (w), DefaultVisualOfScreen (XtScreen (w)), 
                      		1, XYBitmap, 0, bits, width, height, 8, 2);
	XmInstallImage (image, name);
}
