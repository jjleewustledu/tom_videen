/*$Id: sc_bkp3d.c,v 1.2 1995/01/30 16:25:35 ty7777 Exp $*/
/*$Log: sc_bkp3d.c,v $
 * Revision 1.2  1995/01/30  16:25:35  ty7777
 * Added program version.
 **/

/* @(#)sc_bkp3d.c	1.5 8/2/91 */

#include "fproj3d.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/sc_bkp3d.c,v 1.2 1995/01/30 16:25:35 ty7777 Exp $";

sc_bkp3d( vol, vol_info, view, view_info, rot)
  float *vol;
  Vol3d_info vol_info;
  float *view;
  View2d_info view_info;
  float *rot;
{
	int i,j,k,vxd,vyd,vzd,soff,toff,xd;
	unsigned int p,q;
	float x,y,z,s,t,*in,pw,qw,*o;
	float dsdx,dsdy,dsdz,dtdx,dtdy,dtdz;

	dsdx=rot[0];
	dsdy=rot[1];
	dsdz=rot[2];
	dtdx=rot[4];
	dtdy=rot[5];
	dtdz=rot[6];
	xd=view_info->xdim;
	vxd = vol_info->xdim;
	vyd = vol_info->ydim;
	vzd = vol_info->zdim;
	soff=view_info->xdim/2;
	toff=view_info->ydim/2;
	o = vol;
	for (k=0; k<vzd; k++)
	  for (j=0; j<vyd; j++)
	    {
		x = -vxd/2;
		y = j-vyd/2;
		z = (2*k-vzd-1)*0.5;
		s = soff + x*dsdx + y*dsdy + z*dsdz;
		t = toff + x*dtdx + y*dtdy + z*dtdz;
		for (i=0; i<vxd; i++, o++)
		{
			p=s;
			pw=s-p;
			q=t;
			qw=t-q;
			in=view+p+q*xd;
			*(o) += *(in)*(1.-pw)*(1.-qw) +
				*(in+1)*pw*(1.-qw) +
				*(in+xd)*(1.-pw)*qw +
				*(in+xd+1)*pw*qw;
			s += dsdx;
			t += dtdx;
		}
	    }
}
