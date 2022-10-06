/*$Id: xvaln.c,v 1.2 1995/01/30 21:34:50 ty7777 Exp $*/
/*$Log: xvaln.c,v $
 * Revision 1.2  1995/01/30  21:34:50  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xvaln.c,v 1.2 1995/01/30 21:34:50 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvaln_ (y,a,u,b,ny)
#else
void  xvaln_ (y,a,u,b,ny)
#endif /* EMUVERSION */
     
     float   y[], u[], *a, *b;
     long    *ny;
{
  long   i;
  float *uu, *yy;
  
  uu = u;
  yy = y;
  for (i=0,--uu,--yy; i < *ny; i++)
	*++yy= exp (*a * *(++uu) + *b);
}

/*********************************************************************/
