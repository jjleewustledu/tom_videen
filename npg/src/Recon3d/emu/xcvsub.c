/*$Id: xcvsub.c,v 1.3 1995/01/31 16:27:19 ty7777 Exp $*/
/*$Log: xcvsub.c,v $
 * Revision 1.3  1995/01/31  16:27:19  ty7777
 * Moved rcsid to the begining of the file.
 *
 * Revision 1.2  1995/01/30  20:30:04  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xcvsub.c,v 1.3 1995/01/31 16:27:19 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xcvsub_ (y,u,ny)
#else
void  xcvsub_ (y,u,ny)
#endif /* EMUVERSION */

complex y[], u[];
long    *ny;
{
 long   i;

 for (i=0; i < *ny; i++)
     csub (y[i], y[i], u[i]);
}

/*********************************************************************/

