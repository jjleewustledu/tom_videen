/*$Id: xcrmul.c,v 1.3 1995/01/31 16:08:51 ty7777 Exp $*/
/*$Log: xcrmul.c,v $
 * Revision 1.3  1995/01/31  16:08:51  ty7777
 * Moved rcsid to the begining of the file.
 *
 * Revision 1.2  1995/01/30  20:24:57  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xcrmul.c,v 1.3 1995/01/31 16:08:51 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xcrmul_ (a,b,c)
#else
void  xcrmul_ (a,b,c)
#endif /* EMUVERSION */

complex *a, *c;
float   *b;
{
 a->r = *b * c->r;
 a->i = *b * c->i;
}

/*********************************************************************/

