/*$Id: xrmx.c,v 1.2 1995/01/30 20:45:08 ty7777 Exp $*/
/*$Log: xrmx.c,v $
 * Revision 1.2  1995/01/30  20:45:08  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xrmx.c,v 1.2 1995/01/30 20:45:08 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xrmx_ (a,u,ny)
#else
void  xrmx_ (a,u,ny)
#endif /* EMUVERSION */

float   *a, u[];
long    *ny;
{
 long   i;
 float *uu;

 uu = u;
 for (i=1, *a = *uu; i < *ny; i++)
      if (*++uu > *a) 
          *a = *uu;
}

/*********************************************************************/

