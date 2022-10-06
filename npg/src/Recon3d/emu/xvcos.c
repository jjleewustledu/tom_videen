/*$Id: xvcos.c,v 1.2 1995/01/30 21:38:33 ty7777 Exp $*/
/*$Log: xvcos.c,v $
 * Revision 1.2  1995/01/30  21:38:33  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xvcos.c,v 1.2 1995/01/30 21:38:33 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvcos_ (y,a,u,b,ny)
#else
void  xvcos_ (y,a,u,b,ny)
#endif /* EMUVERSION */
     
     float   y[], u[], *a, *b;
     long    *ny;
{
  long   i;
  float *yy, *uu;

  yy = y; 
  uu = u;
  for (i=0, --yy,--uu; i < *ny; i++)
	*++yy = cos (*a * (*++uu) + *b);
}

/*********************************************************************/
