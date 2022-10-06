/*$Id: xvpow.c,v 1.2 1995/01/30 21:54:47 ty7777 Exp $*/
/*$Log: xvpow.c,v $
 * Revision 1.2  1995/01/30  21:54:47  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xvpow.c,v 1.2 1995/01/30 21:54:47 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvpow_ (y,a,u,b,v,ny)
#else
void  xvpow_ (y,a,u,b,v,ny)
#endif /* EMUVERSION */

float   y[], v[], *a, *b;
complex u[];
long    *ny;
{
 long   i;

 for (i=0; i < *ny; i++)
      y[i] = *a * (u[i].r * u[i].r + u[i].i * u[i].i) + *b * v[i];
}

/*********************************************************************/

