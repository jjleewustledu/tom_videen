/*$Id: xfri.c,v 1.2 1995/01/30 20:39:04 ty7777 Exp $*/
/*$Log: xfri.c,v $
 * Revision 1.2  1995/01/30  20:39:04  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xfri.c,v 1.2 1995/01/30 20:39:04 ty7777 Exp $";

#ifdef  EMUVERSION
extern void emu_xfeoi();
extern void emu_xfci_();

void  emu_xfri_ (y,a,u,v,ny)

complex u[], v[];
float   y[], *a;
long    *ny;
{
 emu_xfeoi (v,a,u,ny);
 emu_xfci_ ((complex *)&y[0],v,ny);
}
  
#else
extern void xfeoi();
extern void xfci_();

void  xfri_ (y,a,u,v,ny)

complex u[], v[];
float   y[], *a;
long    *ny;
{
 xfeoi (v,a,u,ny);
 xfci_ ((complex *)&y[0],v,ny);
}
#endif /* EMUVERSION */

/*********************************************************************/

#ifdef EMUVERSION
void emu_xfeoi (v,a,u,ny)
#else
void xfeoi (v,a,u,ny)
#endif /* EMUVERSION */

complex v[], u[];
float   *a;
long    *ny;
{
 float  ang, pi = atan(1.0)*4.0;
 complex temp1, temp2, w;
 complex cbuf, two; 
 long   i, ii;

 two.r = 2.0; two.i = 0.0;
 v[0].r = u[0].r + *a;
 v[0].i = u[0].r - *a;
 conjg(cbuf, u[*ny/2]);
 cmul(v[*ny/2], two, cbuf);

 for (i=0; i < *ny/2-1; i++) {
      ii = *ny - i - 2;
      conjg(cbuf, u[ii+1]);
      cadd(temp1, cbuf, u[i+1]);
      ang = (i+1) * pi / *ny;
      w.r = sin(ang);
      w.i = -cos(ang);
      csub(temp2, cbuf, u[i+1]);
      cmul(temp2, w, temp2);
      csub(cbuf, temp1, temp2);
      conjg(v[ii+1], cbuf);
      cadd(v[i+1], temp1, temp2);
      }
}

/*********************************************************************/

