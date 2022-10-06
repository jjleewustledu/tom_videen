/*$Id: sc_3dbkp.c,v 1.2 1995/01/30 16:24:53 ty7777 Exp $*/
/*$Log: sc_3dbkp.c,v $
 * Revision 1.2  1995/01/30  16:24:53  ty7777
 * Added program version.
 **/

/* @(#)sc_3dbkp.c	1.1 11/11/91 */

#include "scdef.h"
#include "fproj3d.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/sc_3dbkp.c,v 1.2 1995/01/30 16:24:53 ty7777 Exp $";

struct cstype
	{
		float view[512*512];
		float filter[256*256];
		float fftemp[512*512];
		float temp[1024];
		float vol[64*128*128];
		struct view2d_info view_info;
		struct vol3d_info vol_info;
		float rot_data[16];
		float max, min, scaler;
		float zero, one;
	};


void sc_3dbkp( cs)
  struct cstype *cs;
{
	unsigned int i, fft_x, fft_y, fft_xy, fft_y2, one=1, two=2;

	fft_x = cs->view_info.xdim;
	fft_y = cs->view_info.ydim;
	fft_y2 = 2*fft_y;
	fft_xy = fft_x*fft_y;
	cs->zero = 0.0;
	cs->one  = 1.0;
	cs->scaler = 1.0/(float)(fft_x*fft_y);
	for (i=0; i<fft_x; i++)
	{
		xvclr_( cs->temp, &fft_y2, &one);
		xvmov_( cs->temp, &cs->view[i], &fft_y, &two, &fft_x);
		xfcf_( &cs->fftemp[2*i*fft_y], cs->temp, &fft_y);
	}
	for (i=0; i<fft_y; i++)
	{
		xcvmov_( cs->temp, &cs->fftemp[2*i], &fft_x, &one, &fft_y);
		xfcf_( &cs->view[2*i*fft_x], cs->temp, &fft_x);
	}
	xcrml_( cs->view, &cs->scaler, cs->view, cs->filter, &fft_xy);
	for (i=0; i<fft_x; i++)
	{
		xcvmov_( cs->temp, &cs->view[2*i], &fft_y, &one, &fft_x);
		xfci_( &cs->fftemp[2*i*fft_y], cs->temp, &fft_y);
	}
	for (i=0; i<fft_y; i++)
	{
		xcvmov_( cs->temp, &cs->fftemp[2*i], &fft_x, &one, &fft_y);
		xfci_( &cs->view[2*i*fft_x], cs->temp, &fft_x);
	}
	xvmov_( cs->view, cs->view, &fft_xy, &one, &two);
	sc_bkp3d( cs->vol, &cs->vol_info, cs->view, &cs->view_info, cs->rot_data);
}
