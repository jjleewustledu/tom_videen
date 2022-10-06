/*$Id: sc_fp3d.c,v 1.2 1995/01/30 16:26:19 ty7777 Exp $*/
/*$Log: sc_fp3d.c,v $
 * Revision 1.2  1995/01/30  16:26:19  ty7777
 * Added program version.
 **/

/* @(#)sc_fp3d.c	1.1 11/11/91 */

#include "fproj3d.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/sc_fp3d.c,v 1.2 1995/01/30 16:26:19 ty7777 Exp $";

sc_fp3d( vol, vol_info, view, nprojs, nviews, rot)
  float *vol;
  Vol3d_info vol_info;
  float *view;
  int nprojs, nviews;
  float *rot;
{
	float *rot_ptr, *view_ptr;
	unsigned int i;

	for (i=0; i<nviews; i++)
	{
	  rot_ptr = rot+16*i;
	  view_ptr = view+nprojs*i;
	  sc_obview( vol, vol_info, view_ptr, nprojs, rot_ptr);
	}
}
