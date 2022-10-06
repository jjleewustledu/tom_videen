/*$Id: xvsq.c,v 1.2 1995/01/30 22:02:59 ty7777 Exp $*/
/*$Log: xvsq.c,v $
 * Revision 1.2  1995/01/30  22:02:59  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xvsq.c,v 1.2 1995/01/30 22:02:59 ty7777 Exp $";

#ifdef  EMUVERSION
     void  emu_xvsq_ (y,u,ny)
#else
     void  xvsq_ (y,u,ny)
#endif /* EMUVERSION */
     
     float   y[], u[];
     long    *ny;
{
  long   i;
  
  for (i=0; i < *ny; i++)
    y[i] = u[i] * u[i];
}

/*********************************************************************/

