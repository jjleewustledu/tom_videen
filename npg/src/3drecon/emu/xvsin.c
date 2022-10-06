/*$Id: xvsin.c,v 1.2 1995/01/30 21:58:41 ty7777 Exp $*/
/*$Log: xvsin.c,v $
 * Revision 1.2  1995/01/30  21:58:41  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xvsin.c,v 1.2 1995/01/30 21:58:41 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvsin_ (y,a,u,b,ny)
#else
void  xvsin_ (y,a,u,b,ny)
#endif /* EMUVERSION */
     
     float   y[], u[], *a, *b;
     long    *ny;
{
  long   i;
  float *yy, *uu;
  
  yy = y -1; uu = u -1;

  for (i=0; i < *ny; i++)
	*++yy = sin (*a * *++uu + *b);
}

/*********************************************************************/
