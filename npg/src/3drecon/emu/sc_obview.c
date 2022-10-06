/*$Id: sc_obview.c,v 1.2 1995/01/30 16:26:56 ty7777 Exp $*/
/*$Log: sc_obview.c,v $
 * Revision 1.2  1995/01/30  16:26:56  ty7777
 * Added program version.
 **/

/* @(#)sc_obview.c	1.3 11/11/91 */

#include "fproj3d.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/sc_obview.c,v 1.2 1995/01/30 16:26:56 ty7777 Exp $";

sc_obview( vol, vol_info, view, vsize, rot)
  float *vol, *view, *rot;
  Vol3d_info vol_info;
  unsigned int vsize;
{
	float x,y,z,v,wx,wy,wz,*p;
	unsigned int i,j,k,s,t,xd,yd,zd;

	xd = vol_info->xdim;
	yd = vol_info->ydim;
	zd = vol_info->zdim;
	for (t=0; t<vsize; t++)
	{
	  x = rot[3]*vsize+rot[1]*t;
	  y = rot[7]*vsize+rot[5]*t;
	  z = rot[11]*vsize+rot[9]*t;
	  if (z>=0.0) for (s=0; s<vsize; s++)
	  {
		i = x;
		j = y;
		k = z;
		v = 0.0;
		if ((i+1>0)&&(i<xd-1) &&
		    (j+1>0)&&(j<yd-1) &&
		    (k+1>0)&&(k<zd))
		{
		  wx = x-i;
		  wy = y-j;
		  wz = z-k;
		  p = vol+i+j*xd+k*xd*yd;
		  v = (*(p))*(1.-wx)*(1.-wy)*(1.-wz) +
		      (*(p+1))*wx*(1.-wy)*(1.-wz) +
		      (*(p+xd))*(1.-wx)*wy*(1.-wz) +
		      (*(p+xd+1))*wx*wy*(1.-wz) +
		      (*(p+xd*yd))*(1.-wx)*(1.-wy)*wz +
		      (*(p+xd*yd+1))*wx*(1.-wy)*wz +
		      (*(p+xd*yd+xd))*(1.-wx)*wy*wz +
		      (*(p+xd*yd+xd+1))*wx*wy*wz;
		}
		view[s] += v;
		x += rot[0];
		y += rot[4];
		z += rot[8];
	  }
	}
}
