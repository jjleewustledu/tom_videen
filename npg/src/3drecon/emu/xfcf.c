/*$Id: xfcf.c,v 1.2 1995/01/30 20:37:19 ty7777 Exp $*/
/*$Log: xfcf.c,v $
 * Revision 1.2  1995/01/30  20:37:19  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xfcf.c,v 1.2 1995/01/30 20:37:19 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xfcf_ (y,u,ny)
#else
void  xfcf_ (y,u,ny)
#endif /* EMUVERSION */

complex	y[], u[];
long    *ny;
{
#ifdef EMUVERSION
 emu_xf1r4f (y,u,ny);
 emu_xftr4f (y,ny);
#else
 xf1r4f (y,u,ny);
 xftr4f (y,ny);
#endif

 if ((*ny & 1398101) == 0)
#ifdef EMUVERSION
    emu_xflr2f (y,ny);
 else
    emu_xflr4f (y, ny);
#else
    xflr2f (y, ny);
 else
    xflr4f (y, ny);
#endif
}

/*********************************************************************/

#ifdef EMUVERSION
emu_xf1r4f (y,u,ny)
#else
xf1r4f (y,u,ny)
#endif

complex y[], u[];
long    *ny;
{
 complex temp0, temp1, temp2, temp3;
 complex cbuf;
 long   i, j, k;

 for (i = 0; i < *ny; i++) 
      cmov(y[i], u[i]);

 for (j = 0, i = 0; i < *ny-1; i++, j += k) {
      if (i < j) { 
          cmov(temp0, y[j]);
          cmov(y[j], y[i]);
          cmov(y[i], temp0);
	 }
      for (k = *ny/2; k < j+1; k /= 2)
          j -= k;
     }

 for (i = 0; i < *ny; i += 4) {
      cadd(temp0, y[i], y[i+1]);
      csub(temp1, y[i], y[i+1]);
      cadd(temp2, y[i+2], y[i+3]);
      csub(cbuf, y[i+2], y[i+3]);
      temp3.r = 0.0;
      temp3.i = -1.0;
      cmul(temp3, temp3, cbuf);
      cadd(y[i], temp0, temp2);
      cadd(y[i+1], temp1, temp3);
      csub(y[i+2], temp0, temp2);
      csub(y[i+3], temp1, temp3);
     }
}

/**********************************************************************/

#ifdef EMUVERSION
emu_xftr4f (y, ny)
#else
xftr4f (y, ny)
#endif

complex y[];
long    *ny;
{
 float  angle, pi = atan(1.0)*4.0;
 complex ce, ce2, ce3, w;
 complex temp0, temp1, temp2, temp3;
 complex cbuf1, cbuf2;
 long   i, j, k, l, l4, m, i1, i2, i3;

 for (i= *ny, m=0; i>1; i/=2, m++) ;

 for (k = 3; k <= m-2; k += 2) {
      l = 1<<(k-1);
      l4 = 4*l;
      angle = pi/(2.0*l);
      w.r = cos(angle);
      w.i = -sin(angle);
      ce.r = 1.0;
      ce.i = 0.0;
      for (j = 0; j < l; j++) {
           cmul(ce2, ce, ce);
           cmul(ce3, ce, ce2);
           for (i = j; i < *ny; i += l4) {
                i1 = i + l;
                i2 = i1 + l;
                i3 = i2 + l;
                cmul(cbuf1, ce2, y[i1]);
                cadd(temp0, y[i], cbuf1);
                csub(temp1, y[i], cbuf1);
                cmul(cbuf1, ce, y[i2]);
                cmul(cbuf2, ce3, y[i3]);
                cadd(temp2, cbuf1, cbuf2);
                temp3.r = 0.0;
                temp3.i = -1.0;
                csub(cbuf1, cbuf1, cbuf2);
                cmul(temp3, temp3, cbuf1);
                cadd(y[i], temp0, temp2);
                cadd(y[i1], temp1, temp3);
                csub(y[i2], temp0, temp2);
                csub(y[i3], temp1, temp3);
	      }
            cmul(ce, ce, w);
	 }
    }
}

/*********************************************************************/
               
#ifdef EMUVERSION
emu_xflr4f (y,ny)
#else
xflr4f (y,ny)
#endif

complex y[];
long    *ny;
{
 float  angle, pi = atan(1.0)*4.0;
 complex ce, ce2, ce3, w;
 complex temp0, temp1, temp2, temp3;
 complex cbuf1, cbuf2;
 long   k, k1, k2, k3, l;

 angle = 2.0 * pi / *ny;
 w.r = cos(angle);
 w.i = -sin(angle);
 ce.r = 1.0;
 ce.i = 0.0;

 for (l = *ny/4, k = 0; k < l; k++) {
      cmul(ce2, ce, ce);
      cmul(ce3, ce, ce2);
      k1 = k + l;
      k2 = k1 + l;
      k3 = k2 + l;
      cmul(cbuf1, ce2, y[k1]);
      cadd(temp0, y[k], cbuf1);
      csub(temp1, y[k], cbuf1);
      cmul(cbuf1, ce, y[k2]);
      cmul(cbuf2, ce3, y[k3]);
      cadd(temp2, cbuf1, cbuf2);
      temp3.r = 0.0;
      temp3.i = -1.0;
      csub(cbuf1, cbuf1, cbuf2);
      cmul(temp3, temp3, cbuf1);
      cadd(y[k], temp0, temp2);
      cadd(y[k1], temp1, temp3);
      csub(y[k2], temp0, temp2);
      csub(y[k3], temp1, temp3);
      cmul(ce, ce, w);
    }
}

/*********************************************************************/

#ifdef EMUVERSION
emu_xflr2f (y,ny)
#else
xflr2f (y,ny)
#endif

complex y[];
long    *ny;
{
 float  angle, pi = atan(1.0)*4.0;

 complex ce, w, temp;
 long   k, k1;

 angle = 2.0 * pi / *ny;
 w.r = cos(angle);
 w.i = -sin(angle);
 ce.r = 1.0;
 ce.i = 0.0;

 for (k = 0; k < *ny/2; k++) {
      k1 = k + *ny / 2;
      cmul(temp, ce, y[k1]);
      csub(y[k1], y[k], temp);
      cadd(y[k], y[k], temp);
      cmul(ce, ce, w);
    }
}

/*********************************************************************/

