/*$Id: xmstr2str.c,v 1.4 1995/10/30 22:39:01 tom Exp $*/
/*$Log: xmstr2str.c,v $
 * Revision 1.4  1995/10/30  22:39:01  tom
 * change include to remove "petutil"
 *
 * Revision 1.3  1993/11/03  18:12:56  ty7777
 * Added rcsid.
 *
 * Revision 1.2  1993/07/06  18:14:33  ty7777
 * Figure out -D_NO_PROTO problem.
 *
 * Revision 1.1  1993/03/03  20:40:45  ty7777
 * Initial revision
 **/

/***********************************************************************
 * Retrieve the character string from a possibly composite compound string, such as
 * created by xs_concat_words()
 *         From:
 *                   The X Window System, 
 *            Programming and Applications with Xt
 *                   OSF/Motif Edition
 *         by
 *                Douglas Young
 *              Prentice Hall, 1990
 *
 *                 Example described on pages:  ??
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
 *****************************************************************************/

#include <X11/Intrinsic.h> 
#include <X11/StringDefs.h>
#include <Xm/Xm.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libhp/RCS/xmstr2str.c,v 1.4 1995/10/30 22:39:01 tom Exp $";

char *xmstr2str (string) 
	XmString string; 
{ 
	caddr_t			context; 
	char			*text; 
	XmStringCharSet		charset; 
	XmStringDirection	dir; 
	Boolean			separator; 
	char			*buf = NULL; 
	int			done = FALSE;

	XmStringInitContext (&context, string);

	while (!done)
		if (XmStringGetNextSegment (context, &text, &charset, &dir, &separator))
		{ 
			if (separator) /* Stop when next segment is a separator */
				done	= TRUE;

			if (buf)
			{
				buf	= XtRealloc (buf, strlen (buf) + strlen (text) + 2);
				strcat (buf, text);			
			}
			else 
			{
				buf	= (char *) XtMalloc (strlen (text) + 1);
				strcpy (buf, text);
			}
			XtFree (text);
		}
		else
			done	= TRUE;

	XmStringFreeContext (context);

	return buf;
}
