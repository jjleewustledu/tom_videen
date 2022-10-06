/*$Id: xvclip.c,v 1.2 1995/01/30 21:36:06 ty7777 Exp $*/
/*$Log: xvclip.c,v $
 * Revision 1.2  1995/01/30  21:36:06  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xvclip.c,v 1.2 1995/01/30 21:36:06 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvclip_ (y,a,u,b,ny)
#else
     void  xvclip_ (y,a,u,b,ny)
#endif /* EMUVERSION */
     
     float   y[], u[], *a, *b;
     long    *ny;
{
  long   i;
  
  for (i=0; i < *ny; i++)  {
    if (u[i] > *a)
      y[i] = *a;
    else
      y[i] = u[i];
    if (y[i] < *b)  
      y[i] = *b;
  }
}

/*********************************************************************/

