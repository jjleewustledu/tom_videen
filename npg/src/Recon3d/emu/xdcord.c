/*$Id: xdcord.c,v 1.3 1995/01/31 16:29:19 ty7777 Exp $*/
/*$Log: xdcord.c,v $
 * Revision 1.3  1995/01/31  16:29:19  ty7777
 * Moved rcsid to the begining of the file.
 *
 * Revision 1.2  1995/01/30  20:33:50  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xdcord.c,v 1.3 1995/01/31 16:29:19 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xdcord_ (y,id,u,v,ny,nv)
#else
void  xdcord_ (y,id,u,v,ny,nv)
#endif /* EMUVERSION */

float   y[], u[], v[];
long    *id, *ny, *nv;
{
 float  temp, *yy, *vv;
 long   k, j, indu;

 yy = y - 1;

 for (indu=0, k=0; k < *ny; indu += *id, k++) {
 vv = v -1;
      for (temp=0.0, j=0; j < *nv; j++)
           temp += (float)(u[indu+j] * *++vv);
      *++yy = temp;
    }
}

/*********************************************************************/

