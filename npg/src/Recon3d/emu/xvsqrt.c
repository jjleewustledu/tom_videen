/*$Id: xvsqrt.c,v 1.2 1995/01/30 22:03:32 ty7777 Exp $*/
/*$Log: xvsqrt.c,v $
 * Revision 1.2  1995/01/30  22:03:32  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xvsqrt.c,v 1.2 1995/01/30 22:03:32 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvsqrt_ (y,a,u,b,ny)
#else
void  xvsqrt_ (y,a,u,b,ny)
#endif /* EMUVERSION */
     
     float   y[], u[], *a, *b;
     long    *ny;
{
  long   i;
  float *yy, *uu;
  
  yy = y -1; uu = u - 1;
  for (i=0; i < *ny; i++)
	*++yy = sqrt (fabs (*a * *++uu + *b));
}

/*********************************************************************/
