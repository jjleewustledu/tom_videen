/*$Id: xvmvgt.c,v 1.2 1995/01/30 21:52:58 ty7777 Exp $*/
/*$Log: xvmvgt.c,v $
 * Revision 1.2  1995/01/30  21:52:58  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xvmvgt.c,v 1.2 1995/01/30 21:52:58 ty7777 Exp $";

#ifdef EMUVERSION
void emu_xvmvgt_ (y, a, u, b, v, ny)
#else
void xvmvgt_ (y, a, u, b, v, ny)
#endif /* EMUVERSION */
    float y[], a[], u[], *b, v[];
    long *ny;
    {
    long i;
    float *yy, *uu, *vv;

    yy = y - 1; uu = u - 1; vv = v - 1;

    for (i=0; i< *ny; i++)
	if (*++uu - *++vv > *b)
	    *++yy = a[0]* *vv + a[1];
	else
	    *++yy = a[2]* *uu + a[3];
    }
/*********************************************************************/

