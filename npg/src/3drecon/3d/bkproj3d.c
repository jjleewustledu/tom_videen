/*$Id: bkproj3d.c,v 1.13 1995/02/27 21:53:23 ty7777 Exp $*/
/*$Log: bkproj3d.c,v $
 * Revision 1.13  1995/02/27  21:53:23  ty7777
 * Final checking in.
 *
 * Revision 1.12  1995/02/21  17:48:19  ty7777
 * Added processing for merging version numbers.
 *
 * Revision 1.11  1995/02/17  16:53:26  ty7777
 * Added facility name.
 *
 * Revision 1.10  1995/02/03  16:26:39  ty7777
 * The release version.
 *
 * Revision 1.9  1995/02/03  16:20:29  ty7777
 * Added program version.
 *
 * Revision 1.8  1995/02/01  03:57:58  ty7777
 * Added comments.
 *
 * Revision 1.7  1995/01/31  20:13:42  ty7777
 * Deleted checking for file type.
 *
 * Revision 1.6  1995/01/30  22:21:48  ty7777
 * Added processing for merging program versions.
 *
 * Revision 1.5  1995/01/24  21:55:54  ty7777
 * Added program version.
 *
 * Revision 1.4  1995/01/24  19:40:47  ty7777
 * Added petutil.h
 *
 * Revision 1.3  1995/01/24  19:32:18  ty7777
 * Added rcsid and program_version.
 *
 * Revision 1.2  1993/03/15  22:06:51  ty7777
 * 3-D Reconstruction Software Initial Checking in
 *
 * Revision 1.1  1993/03/12  18:17:25  ty7777
 * Initial revision
 **/

#include "matrix.h"
#include "fproj3d.h"
#include "matpkg.h"
#include "scdef.h"
#include <math.h>
#include <string.h>
#include <sys/time.h>

static char* progname;
static char sccs_id [] = "@(#)bkproj3d.c	1.21 10/27/92 Copyright 1991, 1992 CTI Pet Systems, Inc.";
static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/bkproj3d.c,v 1.13 1995/02/27 21:53:23 ty7777 Exp $";

#define REQ_ARGS 6
#define RD_MAX 5
#define NUM_RINGS 16
#define RING_SPACING 0.675
#define RING_DIAMETER 76.0
#define MAX_NUM_RINGS 24
#define MAX_N_SLICES 256
#define MAX_CS 8
#define FAIL -1

static struct cstype
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
	} *cs[MAX_CS];

	float phi0=9.9e-9;
	int ftype_trans, ftype_axial, image_size;
	float cutoff_trans, cutoff_axial, zoom, pixel_size;
	float xyscale, zscale, plane_separation, rot_offset, dead_time_correction;
	int frame_duration, frame_start_time;
	int nprojs, nviews, nslices, fft_x, fft_y, fft_x2, fft_y2, fft_xy;
	int nvoxels, nx, n2dviews=0;
	int fft_xoff, fft_yoff, fft_nv, dtype;
	int one=1, two=2;
	Matrix rot;

csheader()
{};

main( argc, argv)
  int argc;
  char **argv;
{
	extern int GetVersion ();

	FILE		*fptr;
	MatrixData	*scan;
	MatrixFile	*mptr1, *mptr2, *mptr3;
	Main_header	*mh;
	Main_header	mhead;
	Scan_subheader	*ssh;
	char		flag [256], bufr [256];
	char		*measured_file, *synthetic_file, *image_file;
	char		program_version [32];
	float		afit, bfit, aval, bval;
	float		phi, zoff;
	float		*sinos [MAX_N_SLICES], *sino, ring_spacing;
	int		del_sec, del_usec, del_msec, matnum, first, num_cs, x, frame, gate, mode;
	int		i, nv, cachset = 1;
	int		num_rings;
	int		rd, znum, a, b, mflag;
	short int	*sdata;
	struct timeval	t0, t1;

	progname = argv [0];

	if (argc < REQ_ARGS)
	  crash( "%s\nusage: %s %s\n", sccs_id, progname,
		 "measured.scn synthetic.scn afit,bfit[,num_rings,rspace] image_file.img ftype_xy,xy_cutoff,ftype_z,z_cutoff [image_size,zoom,rot_off,zscale] [num_cs,frame,gate]");

	gettimeofday( &t0, 0);
	measured_file = argv [1];
	synthetic_file = argv [2];
	num_rings = NUM_RINGS;
	ring_spacing = RING_SPACING;
	sscanf (argv [3], "%f,%f,%d,%f", &afit, &bfit, &num_rings, &ring_spacing);
	image_file = argv [4];
	phi0 = 0.0;
	sscanf (argv [5], "%d,%f,%d,%f,%f", &ftype_trans, &cutoff_trans, &ftype_axial, &cutoff_axial, &phi0);
	image_size = 0;
	zoom = 1.0;
	rot_offset = 0.0;
	zscale = 1.0;
	if (argc > 5) sscanf (argv [6], "%d,%f,%f,%f", &image_size, &zoom, &rot_offset, &zscale);
	num_cs = 1;
	frame = 1;
	gate = 1;
	if (argc > 6) sscanf (argv [7], "%d,%d,%d", &num_cs, &frame, &gate);
#ifndef NO_CS
	for (i=0; i<num_cs; i++)
	{
	  cs[i] = (struct cstype *) xlubgn_(0, &cachset, "/usr/local/lib/sc.lo");
	  if (cs[i] == (struct cstype*) -1)
	    crash( "%s: SuperCard not available\n", argv [0]);
	  xeintr_(cs[i]);
	}
#else
	cs[0] = (struct cstype*) malloc( sizeof(struct cstype));
	if (!cs[0])
	  crash( "%s: malloc failure for %d bytes\n",
	      argv [0], sizeof( struct cstype));
#endif
	nx = nslices = 2*num_rings-1;
	plane_separation = 0.5*ring_spacing;
	mptr1 = matrix_open (measured_file, MAT_READ_ONLY, SCAN_DATA);
	if (!mptr1)
	  crash ("%s: matrix_open error for %s\n", progname, measured_file);
	ssh = (Scan_subheader*) malloc( sizeof(Scan_subheader));
	mat_read_scan_subheader( mptr1->fptr, mptr1->dirlist->first->strtblk, ssh);
	nprojs = ssh->dimension_1;
	nviews = ssh->dimension_2;
	dead_time_correction = ssh->loss_correction_fctr;
	if (dead_time_correction < 1.0)
	{
	  printf("%s: WARNING...loss correction factor from sinograms = %f\n", progname, dead_time_correction);
	  printf("%s: Setting Dead Time Correction = 1.00\n", progname);
	  dead_time_correction = 1.0;
	}
	else
	  printf("%s: Dead Time Correction = %f\n", progname, dead_time_correction);
	if (image_size == 0) image_size = nprojs;
	xyscale = nprojs/(zoom*image_size);
	pixel_size = ssh->sample_distance*xyscale;
	frame_duration = ssh->frame_duration;
	frame_start_time = ssh->frame_start_time;
	free(ssh);
	nvoxels = image_size*image_size*nslices;
	for (i=0; i<num_cs; i++)
	{
	  xvclr_( cs[i]->vol, &nvoxels, &one);
	  cs[i]->vol_info.xdim = image_size;
	  cs[i]->vol_info.ydim = image_size;
	  cs[i]->vol_info.zdim = nslices;
	}
	mptr2 = matrix_open( synthetic_file, MAT_READ_ONLY, SCAN_DATA);
	if (!mptr2)
	  crash( "%s: can't open matrix file %s\n", progname, synthetic_file);
	mh = matrix_init_main_header (image_file, IMAGE_DATA, mptr1->mhptr);
	mh->plane_separation = plane_separation;
	mh->num_planes = nslices;
	mh->data_type = VAX_I2;
	mode = (frame > 1 || gate > 1) ? MAT_OPEN_EXISTING : MAT_CREATE_NEW_FILE;
	if (GetVersion (rcsid, program_version) == FAIL)
	{
		strncpy (program_version, rcsid + 5, sizeof (mh->physician_name) - 1);
	}

	if (argc > 8) 
	{
		for (i = 8; i <= 9; i++)
		{
			fptr = mat_open (argv [i], "r");
			if (!fptr)
			{
	  			fprintf (stderr, "%s: can't open file %s.\n", argv [0], argv [i]);
			}
			else
			{
				mat_read_main_header (fptr, &mhead);

				if (i == 8) 
				{
					strcat (program_version, " ");
					strcat (program_version, mhead.physician_name);
				}
				else
				{
					strncpy (mh->operator_name, mhead.physician_name, 
					sizeof (mh->operator_name) - 1);
				}
				mat_close (fptr);
			}
		}
	}

	strncpy (mh->physician_name, program_version, sizeof (mh->physician_name) - 1);
	mptr3 = matrix_create (image_file, mode, mh);
	if (!mptr3)
	  crash( "%s: can't create file %s\n", progname, image_file);
	rot = matrix(4,4);
	for (i=0; i<MAX_N_SLICES; i++) sinos[i] = (float*)NULL;
	first=TRUE;
	x = 0;
	while (1)
	{	
		if (!gets( bufr)) break;
		if (*bufr == '*')
		{
		  sscanf( bufr, "*** rd %d, nx %d, phi %g ***",
			&rd, &nx, &phi);
		  fft_x = 64;
		  fft_y = 64;
		  while (fft_x < 2*nprojs) fft_x*=2;
		  while (fft_y < 2*nx) fft_y*=2;
		  if (fft_x*fft_y > 65536)
	  	    crash( "%s: 2d views too large for fft...max product = 65536\n", argv[0]);
		  fft_xoff = (fft_x-nprojs)/2;
		  fft_yoff = (fft_y-nx)/2;
		  fft_x2 = 2*fft_x;
		  fft_y2 = 2*fft_y;
		  fft_xy = fft_x*fft_y;
		  fft_nv = fft_x*fft_y*2;
		  continue;
		}
		else
		{
		  sscanf( bufr, "%d,%d,%f,%f,%d,%s %d,%d",
			&rd, &znum, &phi, &zoff, &matnum, flag, &a, &b);
		}
		if (first)
		{
		  first=FALSE;
		  if (phi0 == 0.0) phi0 = phi<0.0 ? -phi : phi;
		  if (phi0 == 0.0) phi0 = 0.001;
		  printf("%s: phi0 = %.3f degrees\n", progname, phi0);
		}
		if (strncmp( flag, "measured", strlen("measured"))==0) mflag = TRUE;
		else mflag = FALSE;
		scan = matrix_read( mflag?mptr1:mptr2, matnum, MAT_WORD);
		if (!scan)
		  crash("%s: error reading matrix %d from %s\n", progname, matnum, flag);
		sdata = (short int*) scan->data_ptr;
		if (sinos[znum]==NULL) sinos[znum] = (float*) malloc( nprojs*nviews*sizeof(float));
		sino = sinos[znum];
		aval = afit, bval = bfit;
		if (mflag) aval=1.0, bval=0.0;
		for (i=0; i<nprojs*nviews; i++)
		  sino[i] = aval*scan->scale_factor*sdata[i]+bval;
		if (znum == nx-1)
		{
		  printf("%s: processing %d views of %d slices for rd=%d, phi=%0.3f\n", progname, nviews, nx, rd, phi);
		  bkproj_2dviews( cs, num_cs, sinos, nx, nprojs, nviews, rd, -phi, fft_x, fft_y);
		  n2dviews += nviews;
		}
		free( scan->data_ptr);
		free( scan->shptr);
		free( scan);
	}
	store_image( mptr3, num_cs, frame, gate);
	matrix_close( mptr1);
	matrix_close( mptr2);
	matrix_close( mptr3);
	gettimeofday( &t1, 0);
	del_sec = t1.tv_sec-t0.tv_sec;
	del_usec = t1.tv_usec-t0.tv_usec;
	del_msec = 1000*del_sec+del_usec/1000;
	printf("%s: a total of %d 2d views backprojected in %.2f seconds\n",
	  progname, n2dviews, .001*(float)del_msec);

}

bkproj_2dviews( cs, num_cs, sinos, n_slices, nprojs, nviews, rd, phi, fft_x, fft_y)
  struct cstype *cs[];
  float *sinos[];
  int num_cs, n_slices, nprojs, nviews, rd, fft_x, fft_y;
  float phi;
{
	float vbufr[512*512];
	int v, s, x, nbytes;

	nbytes = fft_x*fft_y*sizeof(float);
	bzero( vbufr, nbytes);
	make3dfilter( fft_x, fft_y, M_PI*phi/180., M_PI*phi0/180., vbufr, ftype_trans,
	    cutoff_trans, ftype_axial, cutoff_axial);
	for (x=0; x<num_cs; x++)
	{
	  xldone_(cs[x]);
	  bcopy( vbufr, cs[x]->filter, nbytes);
	  cs[x]->view_info.xdim = fft_x;
	  cs[x]->view_info.ydim = fft_y;
	}
	x = 0;
	for (v=0; v<nviews; v++)
	{
	  xldone_(cs[x]);
	  bzero( cs[x]->view, fft_nv*sizeof(float));
	  for (s=0; s<n_slices; s++)
	    bcopy( sinos[s]+v*nprojs, cs[x]->view+fft_x*(s+fft_yoff)+fft_xoff, nprojs*sizeof(float));
	  compute_rot( rot_offset + 180.*v/nviews, phi, cs[x]->rot_data);
#ifndef NO_CS
	  xccall_( x_f(sc_3dbkp), cs[x], x_p(cs[x]));
#else
	  sc_3dbkp( cs[x]);
#endif
	  x = (x+1)%num_cs;
	}
}

compute_rot( theta, phi, data)
  float theta, phi, *data;
{
	double sint, cost;
	float st, ct, sp, cp;

	/*
	sincos ((double) M_PI * theta / 180.0, &sint, &cost);
	*/
	sint	= sin ((double) M_PI * theta / 180.0);
	cost	= cos ((double) M_PI * theta / 180.0);

	st = sint;
	ct = cost;

	/*
	sincos ((double) M_PI * phi / 180.0, &sint, &cost);
	*/
	sint	= sin ((double) M_PI * phi / 180.0);
	cost	= cos ((double) M_PI * phi / 180.0);

	sp = sint;
	cp = cost;
	data[0] = xyscale*ct;
	data[1] = xyscale*st;
	data[2] = 0.0;
	data[ 3] = 0.0;
	data[ 4] = -xyscale*st*sp;
	data[ 5] = xyscale*ct*sp;
	data[ 6] = zscale*cp;
	data[ 7] = 0.0;
	data[ 8] = xyscale*st*cp;
	data[ 9] = -xyscale*ct*cp;
	data[10] = -zscale*sp;
	data[11] = 0.0;
	data[12] = 0.0;
	data[13] = 0.0;
	data[14] = 0.0;
	data[15] = 1.0;
#ifdef SHOW_ROT
	{
	  static int first=1;
	  if (first)
	  {
	    first=0;
	    printf(" xyscale = %#13.5g     zscale = %#13.5g\n", xyscale, zscale);
	    printf(" Theta     Phi     rot[0]     rot[1]     rot[2]     rot[4]     rot[5]     rot[6]\n");
	  }
	  printf("%#13.5g %#13.5g %#13.5g %#13.5g %#13.5g %#13.5g %#13.5g %#13.5g\n", theta, phi,
		data[0], data[1], data[2], data[4], data[5], data[6]);
	}
#endif SHOW_ROT
}
	
store_image( mptr, num_cs, frame, gate)
  MatrixFile *mptr;
  int num_cs, frame, gate;
{
	MatrixData *image;
	Image_subheader ih;
	short int *ibufr, imin, imax;
	int i, j;
	float scaler, *fp, *fbufr, fmax, fmin, time_scale;

	fbufr = (float*) calloc( nvoxels, sizeof(float));
	for (i=0; i<num_cs; i++)
	{
	  xldone_(cs[i]);
	  for (j=0; j<nvoxels; j++) fbufr[j] += cs[i]->vol[j];
	}
	fmax = fmin = fbufr[0];
	for (i=1; i<nvoxels; i++)
	{
	  if (fbufr[i] > fmax) fmax = fbufr[i];
	  if (fbufr[i] < fmin) fmin = fbufr[i];
	}
	printf("\nImage max = %e\nImage min = %e\n", fmax, fmin);
	ibufr = (short int*) malloc( image_size*image_size*sizeof(short int));
	bzero( &ih, sizeof(Image_subheader));
	image = (MatrixData*)calloc( 1, sizeof(MatrixData));
	image->data_type = MAT_WORD;
	image->data_ptr = (caddr_t)ibufr;
	image->shptr = (caddr_t)&ih;
	image->data_size = image_size*image_size*sizeof(short int);
	ih.data_type = VAX_I2;
	ih.num_dimensions = 2;
	ih.dimension_1 = ih.dimension_2 = image_size;
	ih.recon_scale = zoom;
	ih.pixel_size = pixel_size;
	ih.slice_width = mptr->mhptr->plane_separation;
	ih.frame_duration = frame_duration;
	ih.frame_start_time = frame_start_time;
	ih.loss_corr_fctr = dead_time_correction;

	ih.filter_code		= ftype_trans;
	ih.filter_params [0]	= cutoff_trans;

	time_scale = 1.0;
	if (frame_duration > 0) time_scale = 0.001*frame_duration;
	if (fmax <= 0.0) fmax = 1.0;
	scaler = 32767./fmax;
	for (i=0; i<nslices; i++)
	{
	  fp = fbufr+i*image_size*image_size;
	  imin = imax = 0;
	  for (j=0; j<image_size*image_size; j++)
	  {
	    ibufr[j] = scaler*fp[j];
	    imax = (ibufr[j] > imax) ? ibufr[j] : imax;
	  }
	  ih.quant_scale = dead_time_correction*M_PI/n2dviews/2.0/scaler/time_scale;
	  ih.image_max = imax;
	  ih.image_min = imin;
	  matrix_write( mptr, mat_numcod( frame, i+1, gate, 0, 0), image);
	}
}

minmax( array, n)
  float *array;
  int n;
{
	int i, imax, imin;

	for (i=imax=imin=0; i<n; i++)
	{
	  imax = (array[i]>array[imax]) ? i : imax;
	  imin = (array[i]<array[imin]) ? i : imin;
	}
	printf("Max array[%0d] = %e\n", imax, array[imax]);
	printf("Min array[%0d] = %e\n", imin, array[imin]);
}

#ifdef NO_CS
xldone_()
{};
#endif
