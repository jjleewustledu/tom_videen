/*$Id: make3dfilter.c,v 1.2 1995/01/30 17:02:04 ty7777 Exp $*/
/*$Log: make3dfilter.c,v $
 * Revision 1.2  1995/01/30  17:02:04  ty7777
 * Added program version.
 **/

/* @(#)make3dfilter.c	1.7 9/22/91 */

#include <math.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/make3dfilter.c,v 1.2 1995/01/30 17:02:04 ty7777 Exp $";

make3dfilter( nprojs, nslices, phi, phi0, filter, ftype_x, cutoff_x, ftype_y, cutoff_y)
  int nprojs, nslices, ftype_x, ftype_y;
  float phi, phi0, *filter, cutoff_x, cutoff_y;
{

	float u, v, delta_u, delta_v, rho, rho_sqr, fval, cosphi;
	float cond, sin2_phi, sin2_phi0, dx, dy, damping();
	int x, y;


	cosphi = (float) cos((double)phi);
	sin2_phi = (float) sin((double) phi);
	sin2_phi *= sin2_phi;
	sin2_phi0 = (float) sin((double) phi0);
	sin2_phi0 *= sin2_phi0;
	delta_u = 1.0/(nprojs);
	delta_v = 1.0/(nslices);
	for (y=0; y<=nslices/2; y++)
	{
	  v = y*delta_v;
	  dy = damping( v, ftype_y, cutoff_y);
	  for (x=0; x<=nprojs/2; x++)
	  {
	    u = x*delta_u;
	    dx = damping( u, ftype_x, cutoff_x);
	    if (x+y == 0)
	      filter[0] = 0.0;
	    else
	    {
		rho_sqr = u*u+v*v;
		rho = sqrt(rho_sqr);
		fval = 2.0*rho;
		cond = ((u*u+v*v*sin2_phi)/(rho_sqr*sin2_phi0))-1.0;
		if (cond > 0.0)
		  fval = M_PI*rho/atan(1.0/sqrt(cond));
		fval = fval * dx * dy;
		filter[ x+y*nprojs ] = fval;
		if (x>0) filter[nprojs-x+y*nprojs] = fval;
		if (y>0) filter[x +(nslices-y)*nprojs] = fval;
		if (x*y>0) filter[nprojs-x+(nslices-y)*nprojs] = fval;
	    }
	  }
	}
}

float damping( f, ftype, cutoff)
  float f, cutoff;
  int ftype;
{
	float d;

	switch( ftype)
	{
	  case 1:
		return (f<=cutoff) ? 1.0 : 0.0;
	  case 3:
		d = 0.5+0.5*cos(M_PI*f/cutoff);
		return (f<=cutoff) ? d : 0.0;
	  case 4:
		d = 0.54+0.46*cos(M_PI*f/cutoff);
		return (f<=cutoff) ? d : 0.0;
	  default:
		return (f<=cutoff) ? 1.0 : 0.0;
	}
}

#ifdef TEST

#include <stdio.h>

main( argc, argv)
  int argc;
  char **argv;
{
	float phi, phi0, *filter, cutoff_x, cutoff_y;
	int nprojs, nslices, x, y, ftype_x, ftype_y;
	FILE *optr;

	sscanf( argv[1], "%d,%d,%f,%f,%d,%f,%d,%f", &nprojs, &nslices,
	  &phi, &phi0, &ftype_x, &cutoff_x, &ftype_y, &cutoff_y);
	phi = phi*M_PI/180.;
	phi0 = phi0*M_PI/180.;
	filter = (float*) malloc( nprojs*nslices*sizeof(float));
	make3dfilter( nprojs, nslices, phi, phi0, filter, ftype_x, cutoff_x,
		ftype_y, cutoff_y);
	optr = fopen( argv[2], "w");
	fwrite( filter, nprojs*nslices, sizeof(float), optr);
	close( optr);
}

#endif TEST
