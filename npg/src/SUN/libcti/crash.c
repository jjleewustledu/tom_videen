/*$Id: crash.c,v 1.3 1999/12/13 22:02:19 tom Exp $*/
/*$Log: crash.c,v $
 *Revision 1.3  1999/12/13 22:02:19  tom
 *partially cleaned up with lint
 *
 * Revision 1.2  1996/09/25  16:28:32  tom
 * build in Solaris 2
 *
 * Revision 1.1  1994/03/11  20:40:15  ty7777
 * Initial revision
 **/

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libcti/RCS/crash.c,v 1.3 1999/12/13 22:02:19 tom Exp $";
#endif

#include	<stdio.h>
#include	<stdlib.h>

crash (s0, s1, s2, s3, s4, s5, s6, s7, s8, s9)
	char           *s0, *s1, *s2, *s3, *s4, *s5, *s6, *s7, *s8, *s9;
{
	fprintf (stderr, s0, s1, s2, s3, s4, s5, s6, s7, s8, s9);
	exit (1);
	return (1);
}
