/*$Id: xvfl4.c,v 1.2 1995/01/30 21:47:07 ty7777 Exp $*/
/*$Log: xvfl4.c,v $
 * Revision 1.2  1995/01/30  21:47:07  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xvfl4.c,v 1.2 1995/01/30 21:47:07 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvfl4_ (y,a,u,b,ny)
#else
void  xvfl4_ (y,a,u,b,ny)
#endif /* EMUVERSION */

float   y[], *a, *b;
long    u[], *ny;
{
 long   *uu, i;
 float  *yy;

        yy = y; 
        uu = u;

 for (i=0, --yy, --uu; i < *ny; i++)
      *++yy = *a * *++uu + *b;
}

/*********************************************************************/

