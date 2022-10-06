/*$Id: xcvsbx.c,v 1.3 1995/01/31 16:26:29 ty7777 Exp $*/
/*$Log: xcvsbx.c,v $
 * Revision 1.3  1995/01/31  16:26:29  ty7777
 * Moved rcsid.
 *
 * Revision 1.2  1995/01/30  20:29:26  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xcvsbx.c,v 1.3 1995/01/31 16:26:29 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xcvsbx_ (y,u,ny)
#else
void  xcvsbx_ (y,u,ny)
#endif /* EMUVERSION */

complex	y[], u[];
long    *ny;
{
 long   i;

 for (i=0; i < *ny; i++)
     csub (y[i], u[i], y[i]);
}

/*********************************************************************/

