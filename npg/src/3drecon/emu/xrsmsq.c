/*$Id: xrsmsq.c,v 1.2 1995/01/30 20:47:07 ty7777 Exp $*/
/*$Log: xrsmsq.c,v $
 * Revision 1.2  1995/01/30  20:47:07  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xrsmsq.c,v 1.2 1995/01/30 20:47:07 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xrsmsq_ (a,u,b,nu)
#else
void  xrsmsq_ (a,u,b,nu)
#endif /* EMUVERSION */

float   u[], *a, *b;
long    *nu;
{
    int i;
    for (i=0, *a=0.0; i< *nu; i++)
	*a += u[i] * u[i];
    *a *= *b;
}
/*********************************************************************/

