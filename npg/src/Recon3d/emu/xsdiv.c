/*$Id: xsdiv.c,v 1.2 1995/01/30 20:49:00 ty7777 Exp $*/
/*$Log: xsdiv.c,v $
 * Revision 1.2  1995/01/30  20:49:00  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xsdiv.c,v 1.2 1995/01/30 20:49:00 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xsdiv_ (a,b,c)
#else
void  xsdiv_ (a,b,c)
#endif /* EMUVERSION */

float  *a, *b, *c;
{
 *a = *b / *c;
}

/*********************************************************************/

