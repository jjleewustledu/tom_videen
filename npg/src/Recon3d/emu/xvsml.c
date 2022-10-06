/*$Id: xvsml.c,v 1.2 1995/01/30 22:02:25 ty7777 Exp $*/
/*$Log: xvsml.c,v $
 * Revision 1.2  1995/01/30  22:02:25  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xvsml.c,v 1.2 1995/01/30 22:02:25 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvsml_ (y,a,ny)
#else
void  xvsml_ (y,a,ny)
#endif /* EMUVERSION */

float   y[], *a;
long    *ny;
{
 long   i;
 float *yy;

 yy = y - 1;
 for (i=0; i < *ny; i++)
      *++yy *= *a;
}

/*********************************************************************/

