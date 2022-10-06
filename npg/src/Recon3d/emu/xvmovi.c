/*$Id: xvmovi.c,v 1.2 1995/01/30 21:52:22 ty7777 Exp $*/
/*$Log: xvmovi.c,v $
 * Revision 1.2  1995/01/30  21:52:22  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xvmovi.c,v 1.2 1995/01/30 21:52:22 ty7777 Exp $";

#ifdef  EMUVERSION
extern void emu_xvmov_ () ;
void  emu_xvmovi_ (y,u,iyu,ny,ni,yii,uii)
#else
extern void xvmov_ ();
void  xvmovi_ (y,u,iyu,ny,ni,yii,uii)
#endif /* EMUVERSION */

float   y[], u[];
long    *ny, *yii, *uii, *ni, iyu[][2];
{
 long   i;

 for (i=0; i< *ni; i++)
#ifdef EMUVERSION
     emu_xvmov_  (&y[iyu[i][0]], &u[iyu[i+ *ni][0]], ny, yii, uii);
#else
     xvmov_ (&y[iyu[i][0]], &u[iyu[i+ *ni][0]], ny, yii, uii);
#endif /* EMUVERSION */
}


/*********************************************************************/

