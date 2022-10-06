/*$Id: xcrmai.c,v 1.3 1995/01/31 16:07:53 ty7777 Exp $*/
/*$Log: xcrmai.c,v $
 * Revision 1.3  1995/01/31  16:07:53  ty7777
 * Moved rcsid to begining of the file.
 *
 * Revision 1.2  1995/01/30  20:23:42  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xcrmai.c,v 1.3 1995/01/31 16:07:53 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xcrmai_ (y,a,u,iyu,ia,ni,n)
#else
void  xcrmai_ (y,a,u,iyu,ia,ni,n)
#endif /* EMUVERSION */

long	*ni, *n;
complex y[], u[];
float	a[];
long    iyu[][2], ia[][2];
{
 long   i, k, iy, iu;
 float aa;
 complex t;

 for (i=0; i< *ni; i++)
     for (k=0, iy=iyu[i][0], iu=iyu[*ni+i][0], aa=a[ia[i][0]]; 
          k< *n; 
          iy++, iu++, k++)
         {
	 t.r = u[iu].r * aa;
	 t.i = u[iu].i * aa;
         cadd (y[iy], y[iy], t);
	 }
}

/*********************************************************************/

