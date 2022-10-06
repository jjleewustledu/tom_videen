/*$Id: xrlts.c,v 1.2 1995/01/30 20:43:52 ty7777 Exp $*/
/*$Log: xrlts.c,v $
 * Revision 1.2  1995/01/30  20:43:52  ty7777
 * Added progrma version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xrlts.c,v 1.2 1995/01/30 20:43:52 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xrlts_ (a,u,b,ny)
#else
void  xrlts_ (a,u,b,ny)
#endif /* EMUVERSION */

float   u[], *a, *b;
long    *ny;
{
 long   i;
 float *uu, aa=0.0;

 uu = u -1;

 for (i=0; i < *ny && *++uu >= *b; i++, aa+=1.0) ;
 if (i == *ny)
     *a = -1.0;
 else
     *a = aa;
}

/*********************************************************************/

