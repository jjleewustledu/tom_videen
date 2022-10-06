/*$Id: xssub.c,v 1.2 1995/01/30 21:32:49 ty7777 Exp $*/
/*$Log: xssub.c,v $
 * Revision 1.2  1995/01/30  21:32:49  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xssub.c,v 1.2 1995/01/30 21:32:49 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xssub_ (a,b,c)
#else
void  xssub_ (a,b,c)
#endif /* EMUVERSION */

float  *a, *b, *c;
{
 *a = *b - *c;
}

/*********************************************************************/

