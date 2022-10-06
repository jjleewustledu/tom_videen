/*$Log: getuser.c,v $
 * Revision 1.2  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 1.1  1996/09/26  20:18:32  tom
 * Initial revision
 *
 * Revision 1.2  1993/11/03  17:59:06  ty7777
 * Added rcsid.
 **/

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutilOLD/RCS/getuser.c,v 1.2 1999/12/13 17:04:17 tom Exp $";
#endif

#include <string.h>

char           *getenv ();
char	*getuser (username)
	char           *username;
{
	char           *variable;

	variable = getenv ("USER");
	(void) strcpy (username, variable);
	return (username);
}
