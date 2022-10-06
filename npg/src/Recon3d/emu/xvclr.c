/*$Id: xvclr.c,v 1.2 1995/01/30 21:37:56 ty7777 Exp $*/
/*$Log: xvclr.c,v $
 * Revision 1.2  1995/01/30  21:37:56  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xvclr.c,v 1.2 1995/01/30 21:37:56 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvclr_ (y,ny,yii)
#else
     void  xvclr_ (y,ny,yii)
#endif /* EMUVERSION */
     
     float   y[];
     long    *ny, *yii;
{
  long   i, ind;
  
  for (i=0, ind=0; i < *ny; i++, ind += *yii)
    y[ind] = 0.0;
}

/*********************************************************************/

