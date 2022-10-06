/*$Id: xvdiv.c,v 1.2 1995/01/30 21:39:53 ty7777 Exp $*/
/*$Log: xvdiv.c,v $
 * Revision 1.2  1995/01/30  21:39:53  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xvdiv.c,v 1.2 1995/01/30 21:39:53 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvdiv_ (y,a,u,b,v,c,ny)
#else
void  xvdiv_ (y,a,u,b,v,c,ny)
#endif /* EMUVERSION */

float   y[], u[], v[], *a, *b, *c;
long    *ny;
{
 long   i;
 float  *yy, *uu, *vv;

        yy = y;
        uu = u;
        vv = v;
 
 for (i=0, --uu, --vv, --yy; i < *ny; i++)
      *++yy = *a * (float)((float)(*++vv + *c) / (float)(*++uu + *b));
}

/*********************************************************************/

