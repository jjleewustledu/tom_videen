/*$Id: bkproj.c,v 1.4 1995/01/30 16:02:08 ty7777 Exp $*/
/*$Log: bkproj.c,v $
 * Revision 1.4  1995/01/30  16:02:08  ty7777
 * Added program version.
 *
 * Revision 1.3  1993/03/18  15:40:34  ty7777
 * Write image_type to 2-D attenuation image main header.
 *
 * Revision 1.2  1993/03/15  22:06:51  ty7777
 * 3-D Reconstruction Software Initial Checking in
 **/

#include <stdio.h>
#include <math.h>
#include "matrix.h"
#include <string.h>

#ifndef	lint
static char sccsid[]="@(#)bkproj.c	1.8 4/25/91 Copyright 1991 CTI Pet Systems, Inc.";
#endif	lint

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/bkproj.c,v 1.4 1995/01/30 16:02:08 ty7777 Exp $";

main( argc, argv)
  int argc;
  char *argv[];
{

	short int *scan, *imagei;
	int nprojs, nviews, i, j, size, nblks;
	float *image, *proj, zoom, image_min, image_max;
	float xoff, yoff, rotoff;
	double cost, sint, r0, rdx, rdy, rx0, ry0, theta;
	Main_header mhead, mh2;
	Scan_subheader shead;
	Image_subheader ihead;
	Norm_subheader nhead;
	Attn_subheader ahead;
	float *cbufr, *tbufr, scalef;
	int matnum, frame, plane, gate, sino, bed, bkp_type, bkp_start_view, bkp_nviews;
	struct MatDir entry;
	char fname[256], atn_file[256], nrm_file[256];
	int arg, atn_matnum, nrm_matnum;
	FILE *fptr, *mat_create();
	int fcode, fft_size, fft_size2, fft_offset;
	float fparms[6], *filter_bufr, *fft_bufr, *fft_bufr2, ra, fdummy;
	float fdummy0, offset;


	if (argc < 5)
	  crash( "usage: %s scanspec size[,zoom,xoff,yoff,rot] imagespec filter\n", argv[0]);

	matspec( argv[1], fname, &matnum);
	fptr = mat_open( fname, "r");
	if (!fptr)
	  crash( "%s: can't open input file %s\n", argv[0], fname);
	mat_read_main_header( fptr, &mhead);
	if (mhead.file_type != 1)
	  crash( "%s: matrix file '%s' is not a SCAN file\n", argv[0], fname);
	if (!mat_lookup( fptr, matnum, &entry))
	  crash( "%s: specified matrix not in SCAN file\n", argv[0]);
	mat_read_scan_subheader( fptr, entry.strtblk, &shead);
	nprojs = shead.dimension_1;
	nviews = shead.dimension_2;
	nblks = entry.endblk-entry.strtblk;
	scan = (short int *) malloc( nblks*512);
	mat_read_data( fptr, entry.strtblk+1, 512*nblks, scan, shead.data_type);
	mat_close( fptr);
	size = 128;
	zoom = 1.0;
	xoff = 0.0;
	yoff = 0.0;
	rotoff = 0.0;
	sscanf( argv[2], "%d,%f,%f,%f,%f", &size, &zoom, &xoff, &yoff,&rotoff);
	if (zoom <= 0.5) zoom = 1.0;
	if (size < 32) size = 32;
	ihead.pixel_size = shead.sample_distance*(float)nprojs/((float)size*zoom);
	if (ihead.pixel_size <= 0.0) ihead.pixel_size = 1.0;
	fcode = 0;
	for (i=0; i<6; i++)
	  fparms[i] = 0.0;
	sscanf( argv[4], "%d,%f,%f,%f,%f,%f,%f", &fcode, &fparms[0],
	  &fparms[1], &fparms[2], &fparms[3], &fparms[4], &fparms[5]);
	fft_size = 512;
	while (fft_size < 2*nprojs)
	  fft_size *= 2;
	fft_offset = (fft_size - nprojs) / 2;
	fft_size2 = fft_size/2;
	if (fcode)
	{
	  filter_bufr = (float*) malloc( fft_size * sizeof(float));
	  fft_bufr = (float*) malloc( fft_size * sizeof(float));
	  fft_bufr2 = (float*) malloc( fft_size * sizeof(float));
	  makefilter( fft_size, filter_bufr, fcode, fparms);
	}
	proj = (float*) malloc( fft_size*sizeof(float));
	image = (float*) malloc( size*size*sizeof(float));
	zoom = (float) nprojs / (float) size / zoom;
	cbufr = (float*) malloc( nprojs*nviews*sizeof(float));
	for (i=0; i<nprojs*nviews; cbufr[i++]=1.0);
	scalef = shead.scale_factor;
	if (scalef <= 0.0) scalef = 1.0;
	fdummy = 1.0;
	fdummy0 = 0.5;
	bkp_type = 0;
	bkp_start_view = 0;
	bkp_nviews = nviews;
	arg = 5;
	offset = 0.0;
	while (arg < argc)
	{
	  if (!strcmp( argv[arg], "-b"))
	  { arg++;
	    sscanf( argv[arg++], "%d,%d,%d", &bkp_type, &bkp_start_view,
		&bkp_nviews);
	    continue;
	  }
	  else if (!strcmp( argv[arg], "-o"))
	  {
	    arg++;
	    sscanf( argv[arg++], "%f", &offset);
	    continue;
	  }
	  else if (!strcmp( argv[arg], "-a"))
	  {
	    arg++;
	    matspec( argv[arg++], atn_file, &atn_matnum);
	    fptr = mat_open( atn_file, "r");
	    if (!fptr)
	      crash( "%s: can't open attenuation file '%s'\n", argv[0], atn_file);
	    mat_read_main_header( fptr, &mh2);
	    if (mh2.file_type != 3)
	      crash( "%s: file '%s' is not an ATTEN file\n", argv[0], atn_file);
	    if (!mat_lookup( fptr, atn_matnum, &entry))
	      crash( "%s: attenuation correction matrix not found\n", argv[0]);
	    mat_read_attn_subheader( fptr, entry.strtblk, &ahead);
	    if (( ahead.dimension_1 != nprojs) || (ahead.dimension_2 != nviews))
	      crash( "%s: attenuation correction matrix size mis-match\n", argv[0]);
	    nblks = entry.endblk-entry.strtblk;
	    tbufr = (float*) malloc( nblks*512);
	    mat_read_data( fptr, entry.strtblk+1, 512*nblks, tbufr, ahead.data_type);
	    for (i=0; i<nprojs*nviews; i++)
	      cbufr[i] = cbufr[i] * tbufr[i] ;
	    free( tbufr);
	    scalef = scalef*ahead.scale_factor;
	    continue;
	  }
	  else if (!strcmp( argv[arg], "-n"))
	  {
	    arg++;
	    matspec( argv[arg++], nrm_file, &nrm_matnum);
	    fptr = mat_open( nrm_file, "r");
	    if (!fptr)
	      crash( "%s: can't open normalization file '%s'\n", argv[0], nrm_file);
	    mat_read_main_header( fptr, &mh2);
	    if (mh2.file_type != 4)
	      crash( "%s: file '%s' is not a NORM file\n", argv[0], nrm_file);
	    if (!mat_lookup( fptr, nrm_matnum, &entry))
	      crash( "%s: normalization matrix not found\n", argv[0]);
	    mat_read_norm_subheader( fptr, entry.strtblk, &nhead);
	    if (( nhead.dimension_1 != nprojs) || (nhead.dimension_2 != nviews))
	      crash( "%s: normalization matrix size mis-match\n", argv[0]);
	    nblks = entry.endblk-entry.strtblk;
	    tbufr = (float*) malloc( nblks*512);
	    mat_read_data( fptr, entry.strtblk+1, 512*nblks, tbufr, nhead.data_type);
	    for (i=0; i<nprojs*nviews; i++)
	      cbufr[i] = cbufr[i] * tbufr[i] ;
	    free( tbufr);
	    scalef = scalef*nhead.scale_factor;
	    continue;
	  }
	  else crash( "%s: unknown option '%s'\n", argv[0], argv[arg]);
	}

	for (i=bkp_start_view; i<bkp_nviews; i++)
	{
		theta	= M_PI * i / nviews + rotoff * M_PI / 180.0;

		sint	= sin (theta);
		cost	= cos (theta);

	  	rdx = cost*zoom;
	  	rdy = sint*zoom;
	  for (j=0; j<fft_size; j++)
	    proj[j] = 0.0;
	  for (j=0; j<nprojs; j++)
	    proj[j+fft_offset] = (float) *scan++ * (*cbufr++);
	  if (fcode)
	  {
	    xfrf_( fft_bufr, &ra, proj, &fft_size2);
	    xcvmls_( fft_bufr, &fdummy, fft_bufr, filter_bufr, &fft_size2);
	    ra=ra*filter_bufr[1];
	    xfri_( proj, &ra, fft_bufr, fft_bufr2, &fft_size2);
	  }
	  rx0 = (float) (size/2) - xoff/ihead.pixel_size + offset;
	  ry0 = (float) (size/2) + yoff/ihead.pixel_size + offset;
	  r0 = (float) fft_size2 - rx0*rdx - ry0*rdy;

	  switch (bkp_type)
	  {  case 1:	/* trapazoidal backprojection */
	  	bkproj_view_trap( proj, r0, rdx, rdy, image, size, theta);
		break;
	     case 0:	/* normal */
	     default:
		bkproj_view( proj, r0, rdx, rdy, image, size);
		break;
	  }
	}
	image_max = 0.0;
	image_min = 0.0;
	for (i=0; i<size*size; i++)
	{
	  image_max = (image[i] > image_max) ? image[i] : image_max;
	  image_min = (image[i] < image_min) ? image[i] : image_min;
	}
	imagei = (short int*) malloc( size*size*sizeof(short int));
	for (i=0; i<size*size; i++)
	  imagei[i] = 32767.*image[i]/image_max;
	ihead.data_type = 2;
	ihead.num_dimensions = 2;
	ihead.dimension_1 = ihead.dimension_2 = size;
	ihead.x_origin = xoff;
	ihead.y_origin = yoff;
	ihead.recon_scale = zoom;
	ihead.quant_scale = scalef*M_PI*image_max/32767./nviews/2/fft_size;
	ihead.image_min = 0;
	ihead.image_max = 32767;
	ihead.slice_width = 1.0;

	ihead.filter_code 	= fcode;
	ihead.image_rotation	= 15.0;
	for (i = 0; i < 6; i++)
		ihead.filter_params [i]	= fparms [i];

	mhead.file_type	= 2;
	mhead.data_type	= 2;

	matspec (argv [3], fname, &matnum);
	fptr	= mat_open (fname, "r+");
	if (!fptr) 
		fptr	= mat_create (fname, &mhead);
	mat_write_image (fptr, matnum, &ihead, imagei, size * size * 2);
	mat_close (fptr);
}

bkproj_view( proj, r0, rdx, rdy, image, size)
  float *proj, *image;
  double r0, rdx, rdy;
  int size;
{
	int x, y, j;
	double r, w;

	for (y=0; y<size; y++)
	{
	  r = r0;
	  for (x=0; x<size; x++)
	  {
		j = (int) r;
		w = r-j;
		(*image++) += (1.0-w)*proj[j] + w*proj[j+1];
		r += rdx;
	  }
	  r0 += rdy;
	}
}

bkproj_view_trap( proj, r0, rdx, rdy, image, size, theta)
  float *proj, *image;
  double r0, rdx, rdy, theta;
  int size;
{
	double r, w, w1, w2, r1, r2, r3, r4, f1, f2, f3, q, q1;
	int x,y,b1,b2,b3,b4;
	static int flag=1;

	w1=M_SQRT2*fabs(cos(theta+M_PI/4));
	w2=M_SQRT2*fabs(cos(theta-M_PI/4));
	if (w1>w2) {w=w1;w1=w2;w2=w;}
	f1 = 0.5*(w2-w1)/(w2+w1);
	f2 = 2.0*w1/(w2+w1);
	f3 = f1;
/*
	if (flag) flag=0,printf("theta     w1      w2      f1      f2\n");
	printf("%7.3f  %7.3f   %7.3f   %7.3f   %7.3f\n", theta, w1, w2, f1, f2);
*/
	r0 += 0.5;
	for ( y=0; y< size; y++, r0+=rdy)
	{ 
	  r = r0;
	  for (x=0; x< size; x++, image++, r+=rdx)
	  {
		r1 = r-w2/2;
		b1 = (int) r1;
		r2 = r-w1/2;
		b2 = (int) r2;
		r3 = r+w1/2;
		b3 = (int) r3;
		r4 = r+w2/2;
		b4 = (int) r4;
		if (b1==b2)
		  (*image) += f1*proj[b1];
		else
		{
		  q=b2;
		  q1=(q-r1)*(q-r1)/((r2-r1)*(r2-r1));
		  (*image) += f1*q1*proj[b1]+f1*(1.0-q1)*proj[b2];
		}
		if (b2==b3)
		  (*image) += f2*proj[b2];
		else
		{	
		  q=b3;
		  q1=(q-r2)/(r3-r2);
		  (*image) += f2*q1*proj[b2]+f2*(1.0-q1)*proj[b3];
		}
		if (b3==b4)
		  (*image) += f3*proj[b3];
		else
		{
		  q=b4;
		  q1=(r4-q)*(r4-q)/((r4-r3)*(r4-r3));
		  (*image) += f3*q1*proj[b4]+f3*(1.0-q1)*proj[b3];
		}
	  } /* next x */
	} /* next y */
}
