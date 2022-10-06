/*$Id: xsadd.c,v 1.2 1995/01/30 20:48:24 ty7777 Exp $*/
/*$Log: xsadd.c,v $
 * Revision 1.2  1995/01/30  20:48:24  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xsadd.c,v 1.2 1995/01/30 20:48:24 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xsadd_ (a,b,c)
#else
void  xsadd_ (a,b,c)
#endif /* EMUVERSION */

float  *a, *b, *c;
{
 *a = *b + *c;
}
      
/*********************************************************************/

