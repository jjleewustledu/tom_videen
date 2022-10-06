/*$Id: xvfx2.c,v 1.2 1995/01/30 21:47:51 ty7777 Exp $*/
/*$Log: xvfx2.c,v $
 * Revision 1.2  1995/01/30  21:47:51  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xvfx2.c,v 1.2 1995/01/30 21:47:51 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvfx2_ (y,a,u,b,ny)
#else
void  xvfx2_ (y,a,u,b,ny)
#endif /* EMUVERSION */

float   u[], *a, *b;
short	y[];
long    *ny;
{
 long   i, iy;
 float c;

 for (i=0, iy=0; i < *ny; i+=2, iy+=4)
      {
      c = *a * u[i] + *b;
      y[iy] = c;
      if (c < 0 && y[iy] != c) y[iy] -= 1;

      c = *a * u[i+1] + *b;
      y[iy+1] = c;
      if (c < 0 && y[iy+1] != c) y[iy+1] -= 1;
      }
}

/*********************************************************************/

