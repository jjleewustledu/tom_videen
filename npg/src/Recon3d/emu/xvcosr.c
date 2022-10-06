/*$Id: xvcosr.c,v 1.2 1995/01/30 21:39:14 ty7777 Exp $*/
/*$Log: xvcosr.c,v $
 * Revision 1.2  1995/01/30  21:39:14  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xvcosr.c,v 1.2 1995/01/30 21:39:14 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvcosr_ (y,a,u,b,ny)
#else
void  xvcosr_ (y,a,u,b,ny)
#endif /* EMUVERSION */
     
     float   y[], u[], *a, *b;
     long    *ny;
{
  long   i;
  double twopi = atan (1.0) * 8.0;
  float temp, frac;
  float  *yy, *uu;

      yy = y;
      uu = u; 
  
  for (i=0, --yy, --uu; i < *ny; i++)
      {
      temp = fabs ((*a * (*++uu)+ *b) / twopi);
      frac = temp - (long)temp;
      if ((0.25-frac) > (frac-0.75))
	  *++yy = 0.25 - frac;
      else
	  *++yy = frac - 0.75;
      }
}

/*********************************************************************/
