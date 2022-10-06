/*$Id: xccvml.c,v 1.3 1995/01/30 22:55:27 ty7777 Exp $*/
/*$Log: xccvml.c,v $
 * Revision 1.3  1995/01/30  22:55:27  ty7777
 * No rcsid.
 *
 * Revision 1.2  1995/01/30  20:18:35  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

#ifdef  EMUVERSION
void  emu_xccvml_ (y,a,u,v,ny)
#else
void  xccvml_ (y,a,u,v,ny)
#endif /* EMUVERSION */

complex y[], u[], v[];
float	*a;
long    *ny;
{
 long   i;
 complex t;

 for (i=0; i < *ny; i++) {
     conjg(t, u[i]);
     cmul (y[i], t, v[i]);
     y[i].r *= *a;
     y[i].i *= *a;
    }
}

/*********************************************************************/

