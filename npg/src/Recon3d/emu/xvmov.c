/*$Id: xvmov.c,v 1.2 1995/01/30 21:51:45 ty7777 Exp $*/
/*$Log: xvmov.c,v $
 * Revision 1.2  1995/01/30  21:51:45  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xvmov.c,v 1.2 1995/01/30 21:51:45 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvmov_ (y,u,ny,yii,uii)
#else
void  xvmov_ (y,u,ny,yii,uii)
#endif /* EMUVERSION */

float   y[], u[];
long    *ny, *yii, *uii;
{
 long   i, indy, indu;

 for (i=0, indy=0, indu=0;  i < *ny;  i++, indy += *yii, indu += *uii)
     y[indy] = u[indu];
}

/*********************************************************************/

