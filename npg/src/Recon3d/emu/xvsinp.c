/*$Id: xvsinp.c,v 1.2 1995/01/30 21:59:18 ty7777 Exp $*/
/*$Log: xvsinp.c,v $
 * Revision 1.2  1995/01/30  21:59:18  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xvsinp.c,v 1.2 1995/01/30 21:59:18 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvsinp_ (y,u,ny)
#else
void  xvsinp_ (y,u,ny)
#endif /* EMUVERSION */
     float   y[], u[];
     long    *ny;
{
  register long   i;
  register double twopi = atan (1.0) * 8.0;
  register float *yy;
  register float *uu;
  register long n = *ny / 8;
  
  for (i=0, yy=y - 1, uu=u - 1; i<n; i++)
      {
      *++yy = sin (twopi * *++uu);
      *++yy = sin (twopi * *++uu);
      *++yy = sin (twopi * *++uu);
      *++yy = sin (twopi * *++uu);

      *++yy = sin (twopi * *++uu);
      *++yy = sin (twopi * *++uu);
      *++yy = sin (twopi * *++uu);
      *++yy = sin (twopi * *++uu);
      }
}

/*********************************************************************/
