/*$Id: xvfl2.c,v 1.2 1995/01/30 21:40:40 ty7777 Exp $*/
/*$Log: xvfl2.c,v $
 * Revision 1.2  1995/01/30  21:40:40  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xvfl2.c,v 1.2 1995/01/30 21:40:40 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvfl2_ (y,a,u,b,ny)
#else
void  xvfl2_ (y,a,u,b,ny)
#endif /* EMUVERSION */

float   y[], *a, *b;
short	u[];
long    *ny;
{
 long   i, iu;

 for (i=0, iu=0; i < *ny; i+=2, iu+=4)
      {
      y[i] = *a * u[iu] + *b;
      y[i+1] = *a * u[iu+1] + *b;
      }
}

/*********************************************************************/

