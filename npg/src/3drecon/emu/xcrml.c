/*$Id: xcrml.c,v 1.3 1995/01/31 16:08:18 ty7777 Exp $*/
/*$Log: xcrml.c,v $
 * Revision 1.3  1995/01/31  16:08:18  ty7777
 * Moved rcsid to the begining of the file.
 *
 * Revision 1.2  1995/01/30  20:24:20  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xcrml.c,v 1.3 1995/01/31 16:08:18 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xcrml_ (y,a,u,v,ny)
#else
void  xcrml_ (y,a,u,v,ny)
#endif /* EMUVERSION */

complex y[], u[];
float	v[], *a;
long   *ny;
{
 complex *yy, *uu;
 float *vv;
 long  i;

 yy = y; uu = u; vv = v;
 --yy; --uu; --vv;

 for (i=0; i < *ny; i++) {
     (++yy)->r = *a * (++uu)->r * *++vv;
     yy->i = *a * uu->i * *vv;
   }
}
 
/*********************************************************************/

