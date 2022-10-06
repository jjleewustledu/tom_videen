/*$Id: xsfl4.c,v 1.2 1995/01/30 20:49:35 ty7777 Exp $*/
/*$Log: xsfl4.c,v $
 * Revision 1.2  1995/01/30  20:49:35  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xsfl4.c,v 1.2 1995/01/30 20:49:35 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xsfl4_ (a,b)
#else
void  xsfl4_ (a,b)
#endif /* EMUVERSION */

float   *a;
long    *b;
{
 *a = *b;
}

/*********************************************************************/

