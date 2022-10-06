/*$Id: matspec.c,v 1.3 1999/12/13 22:02:19 tom Exp $*/
/*$Log: matspec.c,v $
 *Revision 1.3  1999/12/13 22:02:19  tom
 *partially cleaned up with lint
 *
 * Revision 1.2  1996/09/25  16:28:32  tom
 * build in Solaris 2
 *
 * Revision 1.1  1994/03/11  20:41:58  ty7777
 * Initial revision
 **/

/*	@(#)matspec.c	2.2	4/24/92	*/


#include <stdio.h>
#include <string.h>
#include <matrix.h>

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libcti/RCS/matspec.c,v 1.3 1999/12/13 22:02:19 tom Exp $";
#endif

matspec (str, fname, matnum)
	char           *str, *fname;
	int            *matnum;
{
	char           *cp;
	int             mf = 0, mp = 0, mg = 0, ms = 0, mb = 0;

	strcpy (fname, str);
	cp = strchr (fname, ',');
	*matnum = 0;
	if (cp) {
		*cp++ = '\0';
		sscanf (cp, "%d,%d,%d,%d,%d", &mf, &mp, &mg, &ms, &mb);
		*matnum = mat_numcod (mf, mp, mg, ms, mb);
	}
	return (0);
}
