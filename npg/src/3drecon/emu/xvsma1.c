/*$Id: xvsma1.c,v 1.2 1995/01/30 22:01:03 ty7777 Exp $*/
/*$Log: xvsma1.c,v $
 * Revision 1.2  1995/01/30  22:01:03  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xvsma1.c,v 1.2 1995/01/30 22:01:03 ty7777 Exp $";

#ifdef  EMUVERSION
void emu_xvsma1_ (y,a,u,b,ny)
#else
void xvsma1_ (y,a,u,b,ny)
#endif /* EMUVERSION */

float   y[], u[], *a, *b;
long    *ny;
{
 long  i;
 float *yy, *uu;

 yy = y - 1; uu = u - 1;

 for (i = 0; i < *ny; i++)
     *++yy = *a * *++uu + *b;
}

/*********************************************************************/

