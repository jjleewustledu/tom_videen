/*$Id: xvsbx.c,v 1.2 1995/01/30 21:58:10 ty7777 Exp $*/
/*$Log: xvsbx.c,v $
 * Revision 1.2  1995/01/30  21:58:10  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xvsbx.c,v 1.2 1995/01/30 21:58:10 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvsbx_ (y,u,ny)
#else
void  xvsbx_ (y,u,ny)
#endif /* EMUVERSION */

float   y[], u[];
long    *ny;
{
 long   i;
 float *yy, *uu;

 uu = u - 1;

 for (i=0, yy=y; i < *ny; i++)
     {
     *yy = *++uu - *yy;
     ++yy;
     }
}

/*********************************************************************/

