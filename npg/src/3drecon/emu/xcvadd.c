/*$Id: xcvadd.c,v 1.3 1995/01/31 16:22:50 ty7777 Exp $*/
/*$Log: xcvadd.c,v $
 * Revision 1.3  1995/01/31  16:22:50  ty7777
 * Moved rcsid to the begining of the file.
 *
 * Revision 1.2  1995/01/30  20:26:55  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xcvadd.c,v 1.3 1995/01/31 16:22:50 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xcvadd_ (y,u,ny)
#else
void  xcvadd_ (y,u,ny)
#endif /* EMUVERSION */

complex y[], u[];
long    *ny;
{
 long   i;

 for (i=0; i < *ny; i++)
     cadd (y[i], y[i], u[i]);
}

/*********************************************************************/

