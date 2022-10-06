/*$Id: xcvcnj.c,v 1.3 1995/01/31 16:23:37 ty7777 Exp $*/
/*$Log: xcvcnj.c,v $
 * Revision 1.3  1995/01/31  16:23:37  ty7777
 * Moved rcsid to the begining of the file.
 *
 * Revision 1.2  1995/01/30  20:27:32  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xcvcnj.c,v 1.3 1995/01/31 16:23:37 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xcvcnj_ (y,u,ny,yii,uii)
#else
void  xcvcnj_ (y,u,ny,yii,uii)
#endif /* EMUVERSION */

complex y[], u[];
long    *ny, *yii, *uii;
{
 long   i, indy, indu;
 complex t;

 for (i=0, indy=0, indu=0;  i < *ny;  i++, indy += *yii, indu += *uii)
     {
     conjg(t, u[indu]);
     cmov (y[indy], t);
     }
}
      
/*********************************************************************/

