/*$Id: xvtabi.c,v 1.2 1995/01/30 22:04:05 ty7777 Exp $*/
/*$Log: xvtabi.c,v $
 * Revision 1.2  1995/01/30  22:04:05  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xvtabi.c,v 1.2 1995/01/30 22:04:05 ty7777 Exp $";

#ifdef EMUVERSION
void emu_xvtabi_ (y, a, u, b, v, ny, nv)
#else
void xvtabi_ (y, a, u, b, v, ny, nv)
#endif /* EMUVERSION */
    float y[], *a, u[], *b, v[];
    long *ny, *nv;
    {
    long i, j;
    float p, f;
    float *yy, *uu;

    yy = y - 1; uu = u - 1;

    for (i=0; i< *ny; i++)
	{
	p = *a * *++uu + *b;
	if (p <= 0.0) 
	    *++yy = v[0];
        else if (p >= *nv-1) 
	    *++yy = v[*nv-1];
	else
	    {
	    j = (long)p;
	    f = p - j;
	    *++yy = (1.0-f) * v[j] + f * v[j+1];
	    }
	}
    }
/*********************************************************************/

