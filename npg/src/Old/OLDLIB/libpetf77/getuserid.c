/*$Id: getuserid.c,v 1.1 1995/10/10 15:35:49 ty7777 Exp $*/
/*$Log: getuserid.c,v $
 * Revision 1.1  1995/10/10  15:35:49  ty7777
 * Initial revision
 **/

#include <stdio.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libpetf77/RCS/getuserid.c,v 1.1 1995/10/10 15:35:49 ty7777 Exp $";

char *getenv ();

getuser_(username) 
char	*username;
{
	char	*variable;

	variable	= getenv ("USER");
	strcpy (username, variable);
}
