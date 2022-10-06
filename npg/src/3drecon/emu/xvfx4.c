/*$Id: xvfx4.c,v 1.2 1995/01/30 21:49:14 ty7777 Exp $*/
/*$Log: xvfx4.c,v $
 * Revision 1.2  1995/01/30  21:49:14  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xvfx4.c,v 1.2 1995/01/30 21:49:14 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvfx4_ (y,a,u,b,ny)
#else
void  xvfx4_ (y,a,u,b,ny)
#endif /* EMUVERSION */

float   u[], *a, *b;
long    y[], *ny;
{
 long   i, *yy;
 float  c, *uu;
 yy = y; uu = u;
 for (i=0, --uu,--yy; i < *ny; i++)
      {
      c = *a * *++uu + *b;
      *++yy = c;
      if (c < 0 && *yy != c) *yy -= 1; 
      }
}

/*********************************************************************/

