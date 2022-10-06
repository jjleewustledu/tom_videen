/*$Id: xvclpu.c,v 1.2 1995/01/30 21:37:21 ty7777 Exp $*/
/*$Log: xvclpu.c,v $
 * Revision 1.2  1995/01/30  21:37:21  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xvclpu.c,v 1.2 1995/01/30 21:37:21 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvclpu_ (y,a,u,ny)
#else
     void  xvclpu_ (y,a,u,ny)
#endif /* EMUVERSION */
     
     float   y[], u[], *a;
     long    *ny;
{
  long   i;
  
  for (i=0; i < *ny; i++)  {
    if (u[i] > *a)
      y[i] = *a;
    else
      y[i] = u[i];
  }
}


