/*$Id: xvad.c,v 1.2 1995/01/30 21:34:07 ty7777 Exp $*/
/*$Log: xvad.c,v $
 * Revision 1.2  1995/01/30  21:34:07  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xvad.c,v 1.2 1995/01/30 21:34:07 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvad_ (y,u,ny)
#else
     void  xvad_ (y,u,ny)
#endif /* EMUVERSION */
     
     float   y[], u[];
     long    *ny;
{
  long   i;
  float  *uu, *yy;
  uu = u;
  yy = y;

  for (i=0, --uu,--yy; i < *ny; i++)
    *++yy += *++uu;
}

/*********************************************************************/

