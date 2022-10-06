/*$Id: xcrma2.c,v 1.4 1995/01/31 17:12:09 ty7777 Exp $*/
/*$Log: xcrma2.c,v $
 * Revision 1.4  1995/01/31  17:12:09  ty7777
 * Added rcsid.
 *
 * Revision 1.3  1995/01/31  16:07:27  ty7777
 * Moved rcsid to begining of the file.
 *
 * Revision 1.2  1995/01/30  20:23:05  ty7777
 * Added program version.
 **/

#include "qcemulib.h"
static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xcrma2.c,v 1.4 1995/01/31 17:12:09 ty7777 Exp $";

complex y[], u[], v[];
#ifdef  EMUVERSION
void  emu_xcrma2_ (y,a,u,b,v,c,ny)
#else
void  xcrma2_ (y,a,u,b,v,c,ny)
#endif /* EMUVERSION */

float   *a, *b, *c;
long    *ny;
{
 register long   i;
 register complex *yy;
 register complex *uu;
 register complex *vv;
 register float aa = *a;
 register float bb = *b;
 register float cc = *c;
 register long   n = *ny / 2;

 yy = y; uu = u; vv = v;

 for (i=0, --yy, --uu, --vv; i<n; i++) 
      {
      (++yy)->r = (aa * (++uu)->r) + (bb * (++vv)->r) + cc;
      yy->i = (aa * uu->i) + (bb * vv->i);
      (++yy)->r = (aa * (++uu)->r) + (bb * (++vv)->r) + cc;
      yy->i = (aa * uu->i) + (bb * vv->i);
      }
}

/*********************************************************************/

