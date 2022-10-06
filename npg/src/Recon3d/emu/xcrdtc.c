/*$Id: xcrdtc.c,v 1.3 1995/01/31 16:06:16 ty7777 Exp $*/
/*$Log: xcrdtc.c,v $
 * Revision 1.3  1995/01/31  16:06:16  ty7777
 * Moved rcsid to begining of the file.
 *
 * Revision 1.2  1995/01/30  20:20:59  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xcrdtc.c,v 1.3 1995/01/31 16:06:16 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xcrdtc_ (a,u,b,v,nu)
#else
void  xcrdtc_ (a,u,b,v,nu)
#endif /* EMUVERSION */

complex *a, u[], v[];
float	*b;
long    *nu;
{
 complex temp, t;
 long   i;

 a->r = 0.0;
 a->i = 0.0;
 for (i=0; i < *nu; i++) {
      conjg(t, u[i]);
      cmul (temp, t, v[i]);
      cadd (*a, *a, temp);
    }
 a->r *= *b;
 a->i *= *b;
}
    
/*********************************************************************/

