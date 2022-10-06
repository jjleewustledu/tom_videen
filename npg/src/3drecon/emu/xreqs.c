/*$Id: xreqs.c,v 1.2 1995/01/30 20:41:26 ty7777 Exp $*/
/*$Log: xreqs.c,v $
 * Revision 1.2  1995/01/30  20:41:26  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xreqs.c,v 1.2 1995/01/30 20:41:26 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xreqs_ (a,u,b,ny)
#else
void  xreqs_ (a,u,b,ny)
#endif /* EMUVERSION */

float   u[], *a, *b;
long    *ny;
{
 long   i;
 float *uu, aa = 0.0;

 uu = u-1;
 for (i=0; i < *ny && *++uu != *b; i++, aa+=1.0) ;
 if (i == *ny)
     *a = -1.0;
 else
     *a = aa;
}

/*********************************************************************/

