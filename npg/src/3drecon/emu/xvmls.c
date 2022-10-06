/*$Id: xvmls.c,v 1.2 1995/01/30 21:51:10 ty7777 Exp $*/
/*$Log: xvmls.c,v $
 * Revision 1.2  1995/01/30  21:51:10  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xvmls.c,v 1.2 1995/01/30 21:51:10 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvmls_ (y,a,u,v,ny)
#else
void  xvmls_ (y,a,u,v,ny)
#endif /* EMUVERSION */

float   y[], u[], v[], *a;
long    *ny; 
{
 long   i;
 float  *uu, *vv, *yy;  

         uu = u;
         vv = v;
         yy = y;
 for (i=0, --uu,--vv,--yy; i < *ny; i++)
      *++yy = *a * *++uu * *++vv;
}

/*********************************************************************/

