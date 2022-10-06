/*$Id: xrsum.c,v 1.2 1995/01/30 20:47:47 ty7777 Exp $*/
/*$Log: xrsum.c,v $
 * Revision 1.2  1995/01/30  20:47:47  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xrsum.c,v 1.2 1995/01/30 20:47:47 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xrsum_ (a,u,b,nu)
#else
void  xrsum_ (a,u,b,nu)
#endif /* EMUVERSION */

float   u[], *a, *b;
long    *nu;
{
    int i;
    float *uu;
  
    uu = u;
    for (i=0, --uu, *a=0.0; i< *nu; i++)
	*a += *++uu;
    *a *= *b;
}
/*********************************************************************/

