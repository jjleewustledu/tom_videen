/*$Id: crash.c,v 1.1 1995/10/16 17:58:12 ty7777 Exp $*/
/*$Log: crash.c,v $
 * Revision 1.1  1995/10/16  17:58:12  ty7777
 * Initial revision
 **/

/* @(#)crash.c	1.1 6/7/90 */

#include <stdio.h>

static char rcsid [] = "$Header: /export/home/npggw/tom/src/libecat7/RCS/crash.c,v 1.1 1995/10/16 17:58:12 ty7777 Exp $";

crash( fmt, a0,a1,a2,a3,a4,a5,a6,a7,a8,a9)
  char *fmt, *a0,*a1,*a2,*a3,*a4,*a5,*a6,*a7,*a8,*a9;
{
	fprintf( stderr, fmt, a0,a1,a2,a3,a4,a5,a6,a7,a8,a9);
	exit(1);
}
