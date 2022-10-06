/*$Id: xvmgtf.c,v 1.2 1995/01/30 21:50:35 ty7777 Exp $*/
/*$Log: xvmgtf.c,v $
 * Revision 1.2  1995/01/30  21:50:35  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xvmgtf.c,v 1.2 1995/01/30 21:50:35 ty7777 Exp $";

#ifdef  EMUVERSION
    void  emu_xvmgtf_ (y,a,w,u,v,ny)
#else
    void      xvmgtf_ (y,a,w,u,v,ny)
#endif /* EMUVERSION */
     
     float   y[], w[], u[], v[], *a;
     long    *ny;

{
  long   i;

for (i=0; i < *ny; i++)  {
    if (v[i] > *a)
        y[i] = w[i];
    else
        y[i] = u[i];
    }

}


