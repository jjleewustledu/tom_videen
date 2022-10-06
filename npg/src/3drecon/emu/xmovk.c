/*$Id: xmovk.c,v 1.2 1995/01/30 20:40:12 ty7777 Exp $*/
/*$Log: xmovk.c,v $
 * Revision 1.2  1995/01/30  20:40:12  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xmovk.c,v 1.2 1995/01/30 20:40:12 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xmovk_ (y,u,nu)
#else
void  xmovk_ (y,u,nu)
#endif /* EMUVERSION */

float y[], u[];
long    *nu;
{
 long   i;

 for (i = 0; i < *nu; i++) {
      y[i] = u[i];
    }
}

/*********************************************************************/

