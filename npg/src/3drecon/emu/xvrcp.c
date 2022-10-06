/*$Id: xvrcp.c,v 1.2 1995/01/30 21:55:21 ty7777 Exp $*/
/*$Log: xvrcp.c,v $
 * Revision 1.2  1995/01/30  21:55:21  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xvrcp.c,v 1.2 1995/01/30 21:55:21 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvrcp_ (y,a,u,b,ny)
#else
void  xvrcp_ (y,a,u,b,ny)
#endif /* EMUVERSION */

float   y[], u[], *a, *b;
long    *ny;
{
 long   i;
 float *yy, *uu;

 yy = y - 1; uu = u - 1;
 for (i=0; i < *ny; i++)
      *++yy = *a / (*++uu + *b);
}

/*********************************************************************/

