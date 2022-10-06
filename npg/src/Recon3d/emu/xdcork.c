/*$Id: xdcork.c,v 1.3 1995/01/31 16:30:36 ty7777 Exp $*/
/*$Log: xdcork.c,v $
 * Revision 1.3  1995/01/31  16:30:36  ty7777
 * Moved rcsid to the begining of the file.
 *
 * Revision 1.2  1995/01/30  20:35:12  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xdcork.c,v 1.3 1995/01/31 16:30:36 ty7777 Exp $";

#ifdef  EMUVERSION
extern  void emu_xmovk_();
extern  void emu_xdcor_();

void  emu_xdcork_ (y,u,v,w,ny,nv)

float   y[], u[], v[], w[];
long    *ny, *nv;
{
 emu_xmovk_ (w,v,nv);
 emu_xdcor_ (y,u,w,ny,nv);
}
#else
extern  void xmovk_();
extern  void xdcor_();

void  xdcork_ (y,u,v,w,ny,nv)

float   y[], u[], v[], w[];
long    *ny, *nv;
{
 xmovk_ (w,v,nv);
 xdcor_ (y,u,w,ny,nv);
}
#endif /* EMUVERSION */

/*********************************************************************/

