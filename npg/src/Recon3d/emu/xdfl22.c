/*$Id: xdfl22.c,v 1.3 1995/01/31 16:31:19 ty7777 Exp $*/
/*$Log: xdfl22.c,v $
 * Revision 1.3  1995/01/31  16:31:19  ty7777
 * Same as before.
 *
 * Revision 1.2  1995/01/30  20:35:48  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xdfl22.c,v 1.3 1995/01/31 16:31:19 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xdfl22_ (y,a,u,b,ny)
#else
     void  xdfl22_ (y,a,u,b,ny)
#endif /* EMUVERSION */
     
     float   y[], u[], a[], b[];
     long    *ny;
{
  long   i;
  
  y[0] = u[0] + (float)((float)((float)(a[0]*b[3]) + (float)(a[1]*b[2])) - 
	 	(float)((float)(a[2]*b[1]) + (float)(a[3]*b[0])));
  y[1] = u[1] + (float)((float)((float)(a[0]*u[0]) + (float)(a[1]*b[3])) - 
		(float)((float)(a[2]*y[0]) + (float)(a[3]*b[1])));

  for (i=2; i < *ny; i++)
   y[i] = u[i] + (float)((float)((float)(a[0]*u[i-1]) + (float)(a[1]*u[i-2])) - 
		  (float)((float)(a[2]*y[i-1]) + (float)(a[3]*y[i-2])));

  b[0] = y[*ny-2];
  b[1] = y[*ny-1];
  b[2] = u[*ny-2];
  b[3] = u[*ny-1];
}

/*********************************************************************/

