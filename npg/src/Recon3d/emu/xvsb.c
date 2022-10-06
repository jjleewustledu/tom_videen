/*$Id: xvsb.c,v 1.2 1995/01/30 21:57:34 ty7777 Exp $*/
/*$Log: xvsb.c,v $
 * Revision 1.2  1995/01/30  21:57:34  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xvsb.c,v 1.2 1995/01/30 21:57:34 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvsb_ (y,u,ny)
#else
void  xvsb_ (y,u,ny)
#endif /* EMUVERSION */

float   y[], u[];
long    *ny;
{
 long   i;
 float *yy, *uu;

 yy = y - 1; uu = u - 1;

 for (i=0; i < *ny; i++)
      *++yy -= *++uu;
}

/*********************************************************************/

