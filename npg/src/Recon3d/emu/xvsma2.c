/*$Id: xvsma2.c,v 1.2 1995/01/30 22:01:41 ty7777 Exp $*/
/*$Log: xvsma2.c,v $
 * Revision 1.2  1995/01/30  22:01:41  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xvsma2.c,v 1.2 1995/01/30 22:01:41 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvsma2_ (y,a,u,b,v,c,ny)
#else
void  xvsma2_ (y,a,u,b,v,c,ny)
#endif /* EMUVERSION */

float   y[], u[], v[], *a, *b, *c;
long    *ny;
{
 register long   i;
 register float *yy;
 register float *uu;
 register float *vv;
 register float aa = *a;
 register float bb = *b;
 register float cc = *c;

 for (i=0, yy=y - 1, uu=u - 1, vv=v - 1; i<*ny; i++)
      {
      *++yy = (aa * *++uu) + (bb * *++vv) + cc;
      }
}

/*********************************************************************/

