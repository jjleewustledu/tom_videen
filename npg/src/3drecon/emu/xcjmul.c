/*$Id: xcjmul.c,v 1.3 1995/01/31 16:02:15 ty7777 Exp $*/
/*$Log: xcjmul.c,v $
 * Revision 1.3  1995/01/31  16:02:15  ty7777
 * Added RCS stuff.
 *
 * Revision 1.2  1995/01/30  20:19:34  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xcjmul.c,v 1.3 1995/01/31 16:02:15 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xcjmul_ (a,b,c)
#else
void  xcjmul_ (a,b,c)
#endif /* EMUVERSION */

complex *a, *b, *c;
{
 conjg(*a, *b);
 cmul(*a, *a, *c);
}

/*********************************************************************/

