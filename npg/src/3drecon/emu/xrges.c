/*$Id: xrges.c,v 1.2 1995/01/30 20:42:03 ty7777 Exp $*/
/*$Log: xrges.c,v $
 * Revision 1.2  1995/01/30  20:42:03  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xrges.c,v 1.2 1995/01/30 20:42:03 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xrges_ (a,u,b,ny)
#else
void  xrges_ (a,u,b,ny)
#endif /* EMUVERSION */

float   u[], *a, *b;
long    *ny;
{
 long   i;
 float *uu, aa=0.0;

 uu = u-1;
 for (i=0; i < *ny && *++uu < *b; i++, aa+=1.0) ;
 if (i == *ny)
     *a = -1.0;
 else
     *a = aa;
}

/*********************************************************************/

