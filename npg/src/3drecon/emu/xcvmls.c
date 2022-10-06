/*$Id: xcvmls.c,v 1.3 1995/01/31 16:24:43 ty7777 Exp $*/
/*$Log: xcvmls.c,v $
 * Revision 1.3  1995/01/31  16:24:43  ty7777
 * Moved rcsid to the begining of the file.
 *
 * Revision 1.2  1995/01/30  20:28:08  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xcvmls.c,v 1.3 1995/01/31 16:24:43 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xcvmls_ (y,a,u,v,ny)
#else
void  xcvmls_ (y,a,u,v,ny)
#endif /* EMUVERSION */

complex y[], u[], v[];
float	*a;
long    *ny;
{
 long   i;

 for (i=0; i < *ny; i++) {
     cmul (y[i], u[i], v[i]);
     y[i].r *= *a;
     y[i].i *= *a;
    }
}

/*********************************************************************/

