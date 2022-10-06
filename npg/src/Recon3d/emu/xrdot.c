/*$Id: xrdot.c,v 1.2 1995/01/30 20:40:49 ty7777 Exp $*/
/*$Log: xrdot.c,v $
 * Revision 1.2  1995/01/30  20:40:49  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xrdot.c,v 1.2 1995/01/30 20:40:49 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xrdot_ (a,u,b,v,nu)
#else
void  xrdot_ (a,u,b,v,nu)
#endif /* EMUVERSION */

float   *a, *b, u[], v[];
long    *nu;
{
 long   i;
 float *uu, *vv;

 uu = u-1; vv = v-1;
 for (i=0, *a=0; i < *nu; i++) 
      *a += (*++uu * *++vv);
 *a *= *b;
}

/*********************************************************************/

