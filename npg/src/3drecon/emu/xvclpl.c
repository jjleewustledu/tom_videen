/*$Id: xvclpl.c,v 1.2 1995/01/30 21:36:44 ty7777 Exp $*/
/*$Log: xvclpl.c,v $
 * Revision 1.2  1995/01/30  21:36:44  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xvclpl.c,v 1.2 1995/01/30 21:36:44 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvclpl_ (y,a,u,ny)
#else
     void  xvclpl_ (y,a,u,ny)
#endif /* EMUVERSION */
     
     float   y[], u[], *a;
     long    *ny;
{
  long   i;
  
  for (i=0; i < *ny; i++)  {
    if (u[i] < *a)
      y[i] = *a;
    else
      y[i] = u[i];
  }
}


