/*$Id: getuserid.c,v 1.2 1993/11/03 17:59:06 ty7777 Exp $*/
/*$Log: getuserid.c,v $
 * Revision 1.2  1993/11/03  17:59:06  ty7777
 * Added rcsid.
 **/

#include <stdio.h>

static char rcsid [] = "$Header: /home/petsun4/ty7777/src/libnpg/RCS/getuserid.c,v 1.2 1993/11/03 17:59:06 ty7777 Exp $";

char *getenv ();

getuser_(username) 
char	*username;
{
	char	*variable;

	variable	= getenv ("USER");
	strcpy (username, variable);
}
