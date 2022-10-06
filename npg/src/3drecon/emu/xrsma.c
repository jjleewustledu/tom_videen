/*$Id: xrsma.c,v 1.2 1995/01/30 20:46:31 ty7777 Exp $*/
/*$Log: xrsma.c,v $
 * Revision 1.2  1995/01/30  20:46:31  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xrsma.c,v 1.2 1995/01/30 20:46:31 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xrsma_ (a,u,b,nu)
#else
void  xrsma_ (a,u,b,nu)
#endif /* EMUVERSION */

float   u[], *a, *b;
long    *nu;
{
    int i;
    float *uu;

    uu = u - 1;

    for (i=0, *a=0.0; i< *nu; i++)
	*a += fabs (*++uu);
    *a *= *b;
}
/*********************************************************************/

