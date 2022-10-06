/*$Id: fproj_volume.c,v 1.2 1995/01/30 16:19:39 ty7777 Exp $*/
/*$Log: fproj_volume.c,v $
 * Revision 1.2  1995/01/30  16:19:39  ty7777
 * Added program version.
 **/

/* @(#)fproj_volume.c	1.22 11/17/92 */

#include <math.h>
#include "matrix.h"
#include "matpkg.h"
#include "fproj3d.h"
#include "scdef.h"
#include <sys/time.h>

static char sccsid[]="@(#)fproj_volume.c	1.22 11/17/92 Copyright 1992 CTI Pet Systems, Inc.";

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/fproj_volume.c,v 1.2 1995/01/30 16:19:39 ty7777 Exp $";

/*
	This procedure performs forward projections of the specified volume.
	The volume and scan files are specified along with sinogram specs on
	the command line. Then forward projection parameters are read from
	stdin, computed and stored in the specified scan matrix until EOF
	(ctrl-D from keyboard).  Forward projection parameters are:

		phi,zoff,matnum.
*/

#define max(a,b) ((a)>(b)?(a):(b))

	static char *prg;

#define MAX_CS 8

static struct cstype
	{
	  struct stack3d_info sinfo;
	  float rot[16*256];
	  float view[256*256];
	  float bufr[128*128];
	  float vol[96*128*128];
	} *cs[MAX_CS];

csheader()
{};

main( argc, argv)
  int argc;
  char **argv;
{
	MatrixFile *mptr;
	MatrixData *scan=NULL;
	Main_header *mh;
	Scan_subheader *sh;
	int rdmax, rd, znum, matnum, a, b;
	float zoff, phi, theta, vol_size, zloc, zoom;
	int i, j, k, nslice, view, cachset=1, out;
	int nprojs, nviews, nx, csi, csx, num_cs=1, x, nbytes, nvals;
	int busy[MAX_CS];
	float bin_size, vx, vy, vz;
	int xd,yd,zd,md, nsinos;
	float *rot, *iptr, *optr, w;
	Matrix cut, compute_matrix();
	struct timeval t0, t1;
	int del_sec, del_usec, del_msec, nv, one=1;
	char flag[256], bufr[256];
	struct stack3d_info sinfo;
	float *vol;

	prg = argv[0];
	if (argc<4)
	{
	  fprintf( stderr, "%s\n", sccsid);
	  crash( "usage: %s vol.img fp.scn nprojs,nviews[,zoom] [num_cs]\n", prg);
	}
	gettimeofday( &t0, 0);
	vol = (float*) malloc( 128*128*96*sizeof(float));
	i = load_stack3d( &sinfo, vol, argv[1], "*");
	if (i==0)
	  crash( "%s: error loading volume from file %s\n", prg, argv[1]);
	xd = sinfo.xdim;
	yd = sinfo.ydim;
	zd = sinfo.zdim;
	md = max(zd,max(xd,yd));
	vx = (xd-1)*sinfo.xy_size;
	vy = (yd-1)*sinfo.xy_size;
	vz = (zd-1)*sinfo.z_size;
	vol_size = max(vz,max(vx,vy));
	mh = matrix_init_main_header( argv[2], SCAN_DATA, NULL);
	mh->data_type = VAX_I2;
	mptr = matrix_create( argv[2], MAT_CREATE, mh);
	if (!mptr) matrix_perror( prg);
	nprojs = 128;
	nviews = 96;
	zoom = 0.0;
	sscanf( argv[3], "%d,%d,%f", &nprojs, &nviews, &zoom);
	if (argc > 4) sscanf( argv[4], "%d", &num_cs);
#ifndef NO_CS
	for (i=0; i<num_cs; i++)
	{
	  cs[i] = (struct cstype*) xlubgn_(0, &cachset, "/usr/local/lib/sc.lo");
	  if (!cs[i])
	    crash( "%s: can't open SuperCard\n", prg);
	  xeintr_(cs[i]);
	  nvals = 96*128*128;
	  xvclr_( cs[i]->vol, &nvals, &one);
	  nvals = 128*128;
	  xvclr_( cs[i]->bufr, &nvals, &one);
	  nbytes = xd*yd*zd*sizeof(float);
	  out = 1;
	  bcopy( vol, cs[i]->vol, nbytes);
	  nbytes = sizeof(sinfo);
	  xlmove_( &sinfo, &cs[i]->sinfo, &nbytes, &out);
	  busy[i] = 0;
	}
#else
	cs[0] = (struct cstype*) malloc( sizeof(struct cstype));
	if (!cs[0])
	  crash("%s: malloc failure for %d bytes\n", prg, sizeof(struct cstype));
	nbytes = xd*yd*zd*sizeof(float);
	bcopy( vol, cs[0]->vol, nbytes);
	bcopy( &sinfo, &cs[0]->sinfo, sizeof(sinfo));
	busy[0] = 0;
#endif
	free( vol);
	gettimeofday( &t1, 0);
	del_sec = t1.tv_sec-t0.tv_sec;
	del_usec = t1.tv_usec-t0.tv_usec;
	del_msec = 1000*del_sec+del_usec/1000;
	rot = (float*) malloc( nviews*16*sizeof(float));
	scan = (MatrixData*) calloc(1, sizeof(MatrixData));
	scan->mat_type = SCAN_DATA;
	sh = (Scan_subheader*) calloc(1, sizeof(Scan_subheader));
	scan->shptr = (caddr_t) sh;
	scan->xdim = nprojs;
	scan->ydim = nviews;
	scan->scale_factor = 1.0;
	scan->data_type = sh->data_type = VAX_I2;
	if (zoom == 0.0) zoom = (float)md/(float)nprojs;
	bin_size = sinfo.xy_size*zoom;
	scan->pixel_size = bin_size;
	printf("%s: volume size = %.2f,%.2f,%.2f cm (%d,%d,%d)\n", prg,
	  vx,vy,vz,xd,yd,zd);
	printf("%s: bin_size = %.3f cm\n", prg, bin_size);
	printf("%s: view width = %.3f cm \n", prg, bin_size*nprojs);
	printf("%s: zoom = %.3f\n", prg, zoom);
	printf("%s: loaded in %0.2f sec.\n", prg, .001*(float)del_msec);
	nsinos = 0;
	x = 0;
	while (1)
	{
		if (!gets( bufr)) break;
		if (*bufr == '*')
		{
		  sscanf( bufr, "*** rd %d, nx %d, phi %g ***",
			&rd, &nx, &phi);
		  continue;
		}
		else
		{
		  sscanf( bufr, "%d,%d,%f,%f,%d,%s %d,%d",
			&rd, &znum, &phi, &zoff, &matnum, flag, &a, &b);
		}
		nsinos++;
		zloc = (zoff/vz+0.5)*(float)(zd-1)/(float)(xd);
		compute_rot( rot, zloc, -phi, nviews, zoom);
		if (busy[x])
		{
		  store_sinogram( cs[x], mptr, scan, busy[x]);
		}
		compute_scan( cs[x], nviews, nprojs, rot);
		busy[x] = matnum;
		x = (x+1)%num_cs;
	}
	for (i=0; i<num_cs; i++)
	{
		if (busy[x])
		{
		  store_sinogram( cs[x], mptr, scan, busy[x]);
		}
		x = (x+1)%num_cs;
	}
	matrix_close( mptr);
	gettimeofday( &t1, 0);
	del_sec = t1.tv_sec-t0.tv_sec;
	del_usec = t1.tv_usec-t0.tv_usec;
	del_msec = 1000*del_sec+del_usec/1000;
	printf("%s: a total of %d sinograms produced in %.2f seconds\n",
	  prg, nsinos, .001*(float)del_msec);
	exit(0);
}

store_sinogram( cs, mptr, scan, matnum)
  struct cstype *cs;
  MatrixFile *mptr;
  Matrix *scan;
  int matnum;
{
	xldone_( cs);
	convert_float_scan( scan, cs->view);
	matrix_write( mptr, matnum, scan);
}

compute_scan( cs, nviews, nprojs, rot)
  struct cstype *cs;
  int nviews, nprojs;
  float *rot;
{
	int nvals, one=1;

	nvals = nviews*16*sizeof(float);
	xlmove_( rot, cs->rot, &nvals, &one);
	nvals = nprojs*nviews;
#ifndef NO_CS
	xvclr_( cs->view, &nvals, &one);
	xccall_( x_f(sc_fp3d), cs, x_p(cs->vol), x_p(&cs->sinfo), x_p(cs->view),
		nprojs, nviews, x_p(cs->rot));
#else
	bzero( cs->view, nvals*sizeof(float));
	sc_fp3d( cs->vol, &cs->sinfo, cs->view, nprojs, nviews, cs->rot);
#endif
}

compute_rot( rot, zoff, phi, nviews, zoom)
  float *rot, zoff, phi, zoom;
  int nviews;
{
	static float *sin_theta=NULL, *cos_theta=NULL;
	int i, j;
	double theta, sint, cost;
	float *r, st, ct, sp, cp;

	if (!sin_theta)
	{
	  sin_theta = (float*) malloc( nviews*sizeof(float));
	  cos_theta = (float*) malloc( nviews*sizeof(float));
	  for (i=0; i<nviews; i++)
	  {
		theta = -M_PI*i/nviews;

		/*
		sincos (theta, &sint, &cost);
		*/
	 	sint	= sin (theta); 	
	 	cost	= cos (theta); 	

		sin_theta[i] = sint;
		cos_theta[i] = cost;
	  }
	}
	/*
	sincos ( (double) M_PI*phi/180., &sint, &cost);
	*/
	sint	= sin ((double) M_PI * phi / 180.0);
	cost	= cos ((double) M_PI * phi / 180.0);
	cp = cost;
	sp = -sint;
	for (i=0; i<nviews; i++)
	{
	  ct = cos_theta[i];
	  st = sin_theta[i];
	  r = rot+16*i;
	  r[ 0] = ct;
	  r[ 1] = cp*st;
	  r[ 2] = -sp*st;
	  r[ 3] = 0.5-0.5*ct+st*(-0.5*cp-zoff*sp);
	  r[ 4] = -st;
	  r[ 5] = cp*ct;
	  r[ 6] = -sp*ct;
	  r[ 7] = 0.5+0.5*st+ct*(-0.5*cp-zoff*sp);
	  r[ 8] = 0.0;
	  r[ 9] = sp;
	  r[10] = cp;
	  r[11] = -0.5*sp+cp*zoff;
	  r[12] = 0.0;
	  r[13] = 0.0;
	  r[14] = 0.0;
	  r[15] = 1.0;
	  for (j=0; j<16; j++) r[j] *= zoom;
	}
}

#ifdef NO_CS
xldone_()
{}
xlmove_( in, out, nbytes, dir)
  char *in, *out;
  int *nbytes, *dir;
{
	if (*dir==1) bcopy( in, out, *nbytes);
	else bcopy( out, in, *nbytes);
}
#endif
