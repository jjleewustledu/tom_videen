/*$Id: xdcori.c,v 1.2 1995/01/30 20:34:35 ty7777 Exp $*/
/*$Log: xdcori.c,v $
 * Revision 1.2  1995/01/30  20:34:35  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xdcori.c,v 1.2 1995/01/30 20:34:35 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xdcori_ (y,u,iu,v,iv,n,ny)
#else
void  xdcori_ (y,u,iu,v,iv,n,ny)
#endif /* EMUVERSION */

float   y[], u[], v[];
long    iu[], iv[], *n, *ny;
{
 float  temp, *yy;
 long   k, j, indiu, indu, indv;

 yy = y - 1;

 for (indiu=0, k=0; k < *ny; indiu += 2, k++) {
      temp = 0.0;  
      indu = iu[indiu];
      indv = iv[indiu];
      for (j=0; j < *n; j++)
           temp += (float)(u[indu+j] * v[indv+j]);
      *++yy = temp;
    }
}

/*********************************************************************/

