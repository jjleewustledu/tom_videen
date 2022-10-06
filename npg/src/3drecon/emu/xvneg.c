/*$Id: xvneg.c,v 1.2 1995/01/30 21:54:14 ty7777 Exp $*/
/*$Log: xvneg.c,v $
 * Revision 1.2  1995/01/30  21:54:14  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xvneg.c,v 1.2 1995/01/30 21:54:14 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvneg_ (y,ny)
#else
     void  xvneg_ (y,ny)
#endif /* EMUVERSION */
     
     float   y[];
     long    *ny;
{
  long   i;
  float *yy;

  
  for (i=0 , yy=y ; i < *ny; i++)
    {
    *yy = -*yy;
    ++yy ;
    }
}

/*********************************************************************/

