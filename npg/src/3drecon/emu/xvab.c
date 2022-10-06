/*$Id: xvab.c,v 1.2 1995/01/30 21:33:26 ty7777 Exp $*/
/*$Log: xvab.c,v $
 * Revision 1.2  1995/01/30  21:33:26  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xvab.c,v 1.2 1995/01/30 21:33:26 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvab_ (y,a,u,b,ny)
#else
     void  xvab_ (y,a,u,b,ny)
#endif /* EMUVERSION */
     
     float   y[], u[], *a, *b;
     long    *ny;
{
  long   i;
  float  *uu, *yy;
  
  uu = u;
  yy = y;
  for (i=0,--uu,--yy; i < *ny; i++)
    if (*++uu<  0.0) 
      *++yy = *b - *a * (*uu);
    else
      *++yy= *a * (*uu) + *b;
}

/*********************************************************************/

