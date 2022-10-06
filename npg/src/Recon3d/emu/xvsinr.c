/*$Id: xvsinr.c,v 1.2 1995/01/30 21:59:52 ty7777 Exp $*/
/*$Log: xvsinr.c,v $
 * Revision 1.2  1995/01/30  21:59:52  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xvsinr.c,v 1.2 1995/01/30 21:59:52 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvsinr_ (y,a,u,b,ny)
#else
void  xvsinr_ (y,a,u,b,ny)
#endif /* EMUVERSION */
     
     float   y[], u[], *a, *b;
     long    *ny;
{
  long   i;
  double twopi = atan (1.0) * 8.0;
  float temp, x1, w, z;
  float *yy, *uu;
  
  yy = y -1; uu = u -1;

  for (i=0; i < *ny; i++)
      {
      x1 = (*a * *++uu + *b) / twopi;
      temp = fabs (x1) - (long)(fabs (x1));
      if (temp < (.5-temp))
	  z = temp;
      else
	  z = .5 - temp;
      if (z > (temp-1.0))
	  w = z;
      else
	  w = temp - 1.0;
      if (x1 < 0)
	  *++yy = -w;
      else
	  *++yy = w;
      }
}

/*********************************************************************/
