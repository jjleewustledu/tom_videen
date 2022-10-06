/*$Id: xvatn2.c,v 1.2 1995/01/30 21:35:30 ty7777 Exp $*/
/*$Log: xvatn2.c,v $
 * Revision 1.2  1995/01/30  21:35:30  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xvatn2.c,v 1.2 1995/01/30 21:35:30 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvatn2_ (y,a,u,b,v,ny)
#else
void  xvatn2_ (y,a,u,b,v,ny)
#endif /* EMUVERSION */
     
     float   y[], u[], v[], *a, *b;
     long    *ny;
{
  long   i;
  float  *yy, *uu, *vv;
  float twopi = atan(1.0)*8.0;
  yy = y; 
  uu = u;
  vv = v;

  for (i=0,--yy,--uu,--vv; i < *ny; i++)
	*++yy= *a / twopi * atan2(*++uu,*++vv) + *b;
}

/*********************************************************************/
