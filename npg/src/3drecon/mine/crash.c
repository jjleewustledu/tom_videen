/*$Id: crash.c,v 1.2 1995/01/30 17:00:28 ty7777 Exp $*/
/*$Log: crash.c,v $
 * Revision 1.2  1995/01/30  17:00:28  ty7777
 * Added program version.
 **/

/* @(#)crash.c	1.1 6/7/90 */

#include <stdio.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/crash.c,v 1.2 1995/01/30 17:00:28 ty7777 Exp $";

crash( fmt, a0,a1,a2,a3,a4,a5,a6,a7,a8,a9)
  char *fmt, *a0,*a1,*a2,*a3,*a4,*a5,*a6,*a7,*a8,*a9;
{
	fprintf( stderr, fmt, a0,a1,a2,a3,a4,a5,a6,a7,a8,a9);
	exit(1);
}
