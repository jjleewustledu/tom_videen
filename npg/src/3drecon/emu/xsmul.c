/*$Id: xsmul.c,v 1.2 1995/01/30 21:32:08 ty7777 Exp $*/
/*$Log: xsmul.c,v $
 * Revision 1.2  1995/01/30  21:32:08  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xsmul.c,v 1.2 1995/01/30 21:32:08 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xsmul_ (a,b,c)
#else
void  xsmul_ (a,b,c)
#endif /* EMUVERSION */

float  *a, *b, *c;
{
 *a = *b * *c;
}

/*********************************************************************/

