/*$Id: xcsadd.c,v 1.3 1995/01/31 16:13:24 ty7777 Exp $*/
/*$Log: xcsadd.c,v $
 * Revision 1.3  1995/01/31  16:13:24  ty7777
 * Moved rcsid to the begining of the file.
 *
 * Revision 1.2  1995/01/30  20:25:38  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xcsadd.c,v 1.3 1995/01/31 16:13:24 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xcsadd_ (a,b,c)
#else
void  xcsadd_ (a,b,c)
#endif /* EMUVERSION */

complex *a, *b, *c;
{
 cadd(*a,*b,*c);
}

/*********************************************************************/

