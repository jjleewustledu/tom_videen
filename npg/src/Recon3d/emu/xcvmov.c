/*$Id: xcvmov.c,v 1.3 1995/01/31 16:25:31 ty7777 Exp $*/
/*$Log: xcvmov.c,v $
 * Revision 1.3  1995/01/31  16:25:31  ty7777
 * Moved rcsid to the begining of the file.
 *
 * Revision 1.2  1995/01/30  20:28:45  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xcvmov.c,v 1.3 1995/01/31 16:25:31 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xcvmov_ (y,u,ny,yii,uii)
#else
void  xcvmov_ (y,u,ny,yii,uii)
#endif /* EMUVERSION */

complex y[], u[];
long    *ny, *yii, *uii;
{
 long   i, indy, indu;

 for (i=0, indy=0, indu=0;  i < *ny;  i++, indy += *yii, indu += *uii)
     cmov (y[indy], u[indu]);
}
      
/*********************************************************************/

