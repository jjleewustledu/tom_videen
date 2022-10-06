/*$Id: xvmvic.c,v 1.2 1995/01/30 21:53:37 ty7777 Exp $*/
/*$Log: xvmvic.c,v $
 * Revision 1.2  1995/01/30  21:53:37  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xvmvic.c,v 1.2 1995/01/30 21:53:37 ty7777 Exp $";

#ifdef  EMUVERSION
extern void emu_xvmov_ ();
void  emu_xvmvic_ (y,u,iyu,ny,ni,yii,uii)
#else
extern void xvmov_();
void  xvmvic_ (y,u,iyu,ny,ni,yii,uii)
#endif /* EMUVERSION */

float   y[], u[];
long    *ny, *yii, *uii, *ni, iyu[];
{
 long   i;

 for (i=0; i< *ni; i++)
#ifdef  EMUVERSION
     emu_xvmov_ (&y[iyu[i]], &u[iyu[i+ *ni]], ny, yii, uii);
#else
     xvmov_ (&y[iyu[i]], &u[iyu[i+ *ni]], ny, yii, uii);
#endif /* EMUVERSION */
}

