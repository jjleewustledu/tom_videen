/*$Id: xgints.c,v 1.2 1995/01/30 20:39:39 ty7777 Exp $*/
/*$Log: xgints.c,v $
 * Revision 1.2  1995/01/30  20:39:39  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xgints.c,v 1.2 1995/01/30 20:39:39 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xgints_ (y,a,ny,yii)
#else
     void  xgints_ (y,a,ny,yii)
#endif /* EMUVERSION */
     
     float   y[], *a;
     long    *ny, *yii;
{
  long   i, ind;
  
  for (i=0, ind=0; i < *ny; i++, ind += *yii)
    y[ind] = *a;
}

/*********************************************************************/

