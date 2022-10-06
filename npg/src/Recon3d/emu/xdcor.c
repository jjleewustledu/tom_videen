/*$Id: xdcor.c,v 1.3 1995/01/31 16:28:09 ty7777 Exp $*/
/*$Log: xdcor.c,v $
 * Revision 1.3  1995/01/31  16:28:09  ty7777
 * Moved rcsid.
 *
 * Revision 1.2  1995/01/30  20:33:00  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xdcor.c,v 1.3 1995/01/31 16:28:09 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xdcor_ (y,u,v,ny,nv)
#else
void  xdcor_ (y,u,v,ny,nv)
#endif /* EMUVERSION */

float   y[], u[], v[];
long    *ny, *nv;
{
 long   i, j;

 for (i=0; i < *ny; i++)
      for (j=0, y[i]=0; j < *nv; j++) 
           y[i] = y[i] + (u[i+j] * v[j]);
}

/*********************************************************************/

