/*$Id: xvsad.c,v 1.2 1995/01/30 21:57:01 ty7777 Exp $*/
/*$Log: xvsad.c,v $
 * Revision 1.2  1995/01/30  21:57:01  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xvsad.c,v 1.2 1995/01/30 21:57:01 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvsad_ (y,a,ny)
#else
     void  xvsad_ (y,a,ny)
#endif /* EMUVERSION */
     
     float   y[], *a;
     long    *ny;
{
  long   i;
  float *yy;
  
  yy = y -1;
  for (i=0; i < *ny; i++)
    *++yy += *a;
}

/*********************************************************************/

