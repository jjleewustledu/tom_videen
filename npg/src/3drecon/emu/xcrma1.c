/*$Id: xcrma1.c,v 1.3 1995/01/31 16:06:53 ty7777 Exp $*/
/*$Log: xcrma1.c,v $
 * Revision 1.3  1995/01/31  16:06:53  ty7777
 * Moved rcsid to begining of the file.
 *
 * Revision 1.2  1995/01/30  20:22:00  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xcrma1.c,v 1.3 1995/01/31 16:06:53 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xcrma1_ (y,a,u,b,ny)
#else
void  xcrma1_ (y,a,u,b,ny)
#endif /* EMUVERSION */

complex y[], u[];
float	*a, *b;
long    *ny;
{
 long   i;

 for (i=0; i < *ny; i++) {
      y[i].r = *a * u[i].r + *b;
      y[i].i = *a * u[i].i;
    }
}

/*********************************************************************/

