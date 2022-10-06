/*$Id: xsfx4.c,v 1.2 1995/01/30 21:31:11 ty7777 Exp $*/
/*$Log: xsfx4.c,v $
 * Revision 1.2  1995/01/30  21:31:11  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xsfx4.c,v 1.2 1995/01/30 21:31:11 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xsfx4_ (a,b)
#else
void  xsfx4_ (a,b)
#endif /* EMUVERSION */

long   *a;
float  *b;
{
 *a = *b;
 if (*b < 0 && *b != *a) *a -= 1;
}

/*********************************************************************/

