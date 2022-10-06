/*$Id: xccmul.c,v 1.4 1995/01/31 16:04:50 ty7777 Exp $*/
/*$Log: xccmul.c,v $
 * Revision 1.4  1995/01/31  16:04:50  ty7777
 * Added static variable.
 *
 * Revision 1.3  1995/01/30  22:53:14  ty7777
 * get rid of rcsid.
 *
 * Revision 1.2  1995/01/30  20:17:25  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xccmul.c,v 1.4 1995/01/31 16:04:50 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xccmul_ (a,b,c)
#else
void  xccmul_ (a,b,c)
#endif /* EMUVERSION */

complex *a, *b, *c;
{
 cmul(*a, *b, *c);
}

/*********************************************************************/

