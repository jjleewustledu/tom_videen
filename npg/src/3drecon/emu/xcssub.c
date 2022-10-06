/*$Id: xcssub.c,v 1.3 1995/01/31 16:20:12 ty7777 Exp $*/
/*$Log: xcssub.c,v $
 * Revision 1.3  1995/01/31  16:20:12  ty7777
 * Moved rcsid to the begining of the file.
 *
 * Revision 1.2  1995/01/30  20:26:15  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xcssub.c,v 1.3 1995/01/31 16:20:12 ty7777 Exp $";


#ifdef  EMUVERSION
void  emu_xcssub_ (a,b,c)
#else
void  xcssub_ (a,b,c)
#endif /* EMUVERSION */

complex *a, *b, *c;
{
 csub(*a,*b,*c);
}

/*********************************************************************/

