/*$Id: xvln.c,v 1.2 1995/01/30 21:49:53 ty7777 Exp $*/
/*$Log: xvln.c,v $
 * Revision 1.2  1995/01/30  21:49:53  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xvln.c,v 1.2 1995/01/30 21:49:53 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvln_ (y,a,u,b,ny)
#else
void  xvln_ (y,a,u,b,ny)
#endif /* EMUVERSION */
     
     float   y[], u[], *a, *b;
     long    *ny;
{
  long   i;
  float  *yy, *uu;

         yy = y;
         uu = u; 

  for (i=0, --uu, --yy; i < *ny; i++)
	*++yy = *a * log (fabs (*++uu)) + *b;
}

/*********************************************************************/
