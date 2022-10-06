/*$Id: xcrdot.c,v 1.3 1995/01/31 16:05:32 ty7777 Exp $*/
/*$Log: xcrdot.c,v $
 * Revision 1.3  1995/01/31  16:05:32  ty7777
 * Moved rcsid to begining of the file.
 *
 * Revision 1.2  1995/01/30  20:20:14  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xcrdot.c,v 1.3 1995/01/31 16:05:32 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xcrdot_ (a,u,b,v,nu)
#else
void  xcrdot_ (a,u,b,v,nu)
#endif /* EMUVERSION */

complex *a, u[], v[];
float	*b;
long    *nu;
{
 complex temp;
 long   i;

 a->r = 0.0;
 a->i = 0.0;
 for (i=0; i < *nu; i++) {
      cmul (temp, u[i], v[i]);
      cadd (*a, *a, temp);
    }
 a->r *= *b;
 a->i *= *b;
}
    
/*********************************************************************/

