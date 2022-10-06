/*$Id: xdintg.c,v 1.2 1995/01/30 20:36:38 ty7777 Exp $*/
/*$Log: xdintg.c,v $
 * Revision 1.2  1995/01/30  20:36:38  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xdintg.c,v 1.2 1995/01/30 20:36:38 ty7777 Exp $";

#ifdef  EMUVERSION
void emu_xdintg_ (y,a,u,b,ny)
#else
void xdintg_ (y,a,u,b,ny)
#endif /* EMUVERSION */

float   y[], u[], *a, *b;
long    *ny;
{
 long  i;

 for (i = 1, y[0] = u[0] + *a; i < *ny; i++)
     y[i] = u[i] + y[i-1];
 *b = y[*ny-1];
}

/*********************************************************************/

