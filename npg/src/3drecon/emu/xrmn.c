/*$Id: xrmn.c,v 1.2 1995/01/30 20:44:28 ty7777 Exp $*/
/*$Log: xrmn.c,v $
 * Revision 1.2  1995/01/30  20:44:28  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xrmn.c,v 1.2 1995/01/30 20:44:28 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xrmn_ (a,u,ny)
#else
void  xrmn_ (a,u,ny)
#endif /* EMUVERSION */

float   *a, u[];
long    *ny;
{
 long   i;
 float *uu;

 uu = u;
 for (i = 1, *a = *uu; i < *ny; i++){
      if (*++uu < *a)
          *a = *uu;
 }
}

/*********************************************************************/

