/*$Id: xfrf.c,v 1.2 1995/01/30 20:38:30 ty7777 Exp $*/
/*$Log: xfrf.c,v $
 * Revision 1.2  1995/01/30  20:38:30  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xfrf.c,v 1.2 1995/01/30 20:38:30 ty7777 Exp $";

#ifdef  EMUVERSION
extern void emu_xfcf_();
extern void emu_xfeof();

void  emu_xfrf_ (y,a,u,ny)

complex y[];
float   *a, u[];
long    *ny;
{
 emu_xfcf_ (y,(complex *)&u[0],ny);
 emu_xfeof (y,a,ny);
}
  
#else
extern void xfcf_();
extern void xfeof();

void  xfrf_ (y,a,u,ny)

complex y[];
float   *a, u[];
long    *ny;
{
 xfcf_ (y,(complex *)&u[0],ny);
 xfeof (y,a,ny);
}
#endif /* EMUVERSION */

/*********************************************************************/

#ifdef EMUVERSION
void emu_xfeof (y,a,ny)
#else
void xfeof (y,a,ny)
#endif /* EMUVERSION */

complex y[];
float   *a;
long    *ny;
{
 float  ang, pi = atan(1.0)*4.0;
 complex temp1, temp2, w;
 complex cbuf, two; 
 long   i, ii;

 two.r = 2.0; two.i = 0.0;
 *a = 2.0 * (y[0].r - y[0].i);
 y[0].r = 2.0 * (y[0].r + y[0].i);
 y[0].i = 0.0;
 conjg(cbuf, y[*ny/2]);
 cmul(y[*ny/2], two, cbuf);

 for (i=0; i < *ny/2-1; i++) {
      ii = *ny - i - 2;
      conjg(cbuf, y[ii+1]);
      cadd(temp1, cbuf, y[i+1]);
      ang = (i+1) * pi / *ny;
      w.r = sin(ang);
      w.i = cos(ang);
      csub(temp2, cbuf, y[i+1]);
      cmul(temp2, w, temp2);
      csub(cbuf, temp1, temp2);
      conjg(y[ii+1], cbuf);
      cadd(y[i+1], temp1, temp2);
      }
}

/*********************************************************************/

