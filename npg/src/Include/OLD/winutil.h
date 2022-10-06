/*$Id: winutil.h,v 1.2 1995/10/30 22:47:21 tom Exp $*/
/*$Log: winutil.h,v $
 * Revision 1.2  1995/10/30  22:47:21  tom
 * remove reference to path petutil
 *
 * Revision 1.1  1995/10/30  18:18:57  tom
 * Initial revision
 *
 * Revision 1.9  1994/10/25  18:35:26  ty7777
 * Added Constants.
 *
 * Revision 1.8  1994/10/20  19:05:21  ty7777
 * Added CreateXorGC ().
 *
 * Revision 1.7  1994/03/03  21:04:17  ty7777
 * Replace id with header.
 *
 * Revision 1.6  1994/01/21  20:11:46  ty7777
 * Added LoadXwd, WindowDump, DoPseudo.
 *
 * Revision 1.5  1993/12/16  21:56:24  ty7777
 * Added REDRAW, SELECT, CANCEL.
 *
 * Revision 1.4  1993/12/15  22:44:48  ty7777
 * Added ALIGNMENT and ORIENT constants.
 *
 * Revision 1.3  1993/12/08  19:41:13  ty7777
 * Changed : to ;.
 *
 * Revision 1.2  1993/11/11  17:55:30  ty7777
 * Working version, initial checking in.
 **/

/*________________________________________________________________________ 
File:		winutil.h

Description:	General definition and includes for X window and Motif 
		related utilities.

Author:		Tom Yang

Date:		09/17/93
________________________________________________________________________*/ 

#ifndef WINUTIL_INCLUDED
#define WINUTIL_INCLUDED

static char rcsid_winutil [] = "$Header: /home/npggw/tom/inc/RCS/winutil.h,v 1.2 1995/10/30 22:47:21 tom Exp $";

#include <petutil.h>
#include <X11/StringDefs.h>
#include <X11/Intrinsic.h> 


/* 
 * constants
 */
#define ALIGNMENT_LEFT		0		/* alignment left */
#define ALIGNMENT_CENTER	1		/* alignment center */
#define ALIGNMENT_RIGHT		2		/* alignment right */
#define ALIGNMENT_NUM		3		/* alignment left, center, right */
#define ORIENT_HORI		0		/* horizontal */
#define ORIENT_VERT		1		/* vertical */
#define ORIENT_NUM		2		/* orientation number */
#define REDRAW			0		/* redraw */
#define SELECT			1		/* select */
#define CANCEL			2		/* cancel */
#define BITMAP_WIDTH		16
#define BITMAP_HEIGHT		16
#define MAX_SYMBOL_NUM		7
#define FILLED_SYMBOL_START	7
#define RECTANGLE		0
#define CIRCLE			1
#define DIAMOND			2
#define UP_TRIANGLE		3
#define DOWN_TRIANGLE		4
#define LEFT_TRIANGLE		5
#define RIGHT_TRIANGLE		6
#define FILLED_RECTANGLE	7
#define FILLED_CIRCLE		8
#define FILLED_DIAMOND		9
#define FILLED_UP_TRIANGLE	10
#define FILLED_DOWN_TRIANGLE	11
#define FILLED_LEFT_TRIANGLE	12
#define FILLED_RIGHT_TRIANGLE	13
#define CROSSHAIR		14


/* 
 * structures
 */
typedef struct 
{
	char	*label;		/* PushButton's label */
	void	(*callback)();	/* pointer to a callback routine, when activated */
	caddr_t	data;		/* client data for the callback routine */
} ActionAreaItem;


/* 
 * Global functions. 
 */
PUBLIC	GC	CreateXorGC ();
PUBLIC	void	GetTextStart ();
PUBLIC	void	LoadXwd ();
PUBLIC  void	WindowDump ();
PUBLIC	void	DoPseudo ();

#endif /* WINUTIL_INCLUDED */
