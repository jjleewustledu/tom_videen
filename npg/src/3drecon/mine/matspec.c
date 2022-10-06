/*$Id: matspec.c,v 1.2 1995/01/30 17:07:02 ty7777 Exp $*/
/*$Log: matspec.c,v $
 * Revision 1.2  1995/01/30  17:07:02  ty7777
 * Added program version.
 **/

/* @(#)matspec.c	1.1 6/8/90 */

#include <string.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/matspec.c,v 1.2 1995/01/30 17:07:02 ty7777 Exp $";

matspec (str, fname, matnum)
  char *str, *fname;
  int *matnum;
{
	char	*cp;
	int	mf = 0, mp = 0, mg = 0, ms = 0, mb = 0;

	strcpy (fname, str);
	cp = strchr (fname, ',');
	*matnum = 0;
	if (cp)
	{
		*cp++	= '\0';
		sscanf (cp, "%d,%d,%d,%d,%d", &mf, &mp, &mg, &ms, &mb);
		*matnum	= mat_numcod (mf, mp, mg, ms, mb);
	}
}
		
