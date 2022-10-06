/*$Id: xvfl2c.c,v 1.2 1995/01/30 21:46:27 ty7777 Exp $*/
/*$Log: xvfl2c.c,v $
 * Revision 1.2  1995/01/30  21:46:27  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xvfl2c.c,v 1.2 1995/01/30 21:46:27 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvfl2c_ (y,a,u,b,ny)
#else
void  xvfl2c_ (y,a,u,b,ny)
#endif /* EMUVERSION */

float   y[], *a, *b;
short	u[];
long    *ny;
{
 long   i, iu;

 for (i=0, iu=0; i < *ny; i+=2, iu+=2)
      {
      y[i] = *a * u[iu] + *b;
      y[i+1] = *a * u[iu+1] + *b;
      }
}

/*********************************************************************/

