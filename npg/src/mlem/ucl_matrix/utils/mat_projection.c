/* @(#)matcopy.c	1.4 7/10/92 */
/* 09-Nov-1995 : modified by sibomana@topo.ucl.ac.be */

#include "sys/types.h"
#include "load_volume.h"

#define SHORT_MAX 32767

#ifndef lint
static char sccsid[]="@(#)matcopy.c	1.4 7/10/92 Copyright 1990 CTI Pet Systems, Inc.";
#endif lint

typedef enum {
		Dimension_X, Dimension_Y, Dimension_Z, Dimension_Undefined
	} DimensionName;

static short *svol=NULL, *splane=NULL, *sline=NULL;
static u_char *bvol=NULL, *bplane=NULL, *bline=NULL;
static usage() {
	fprintf(stderr,
		"usage: mat_projection -i in_matspec -o out_matspec -t|-c|-s -l min,max [-v]\n");
	fprintf(stderr,"-t for transverse projection or \n");
	fprintf(stderr,"-c for coronal projection or \n");
	fprintf(stderr,"-s for sagittal projection or \n");
	fprintf(stderr,"-v set verbose mode on ( default is off)\n");
	exit(1);
}

int *x_projection(volume,x0,x1)
MatrixData* volume;
int x0, x1;
{
	int *sum, *sum_line;
	int  x,y,z;
	int sx = volume->xdim, sy = volume->ydim, sz = volume->zdim;

	sum = (int*)calloc(sy*sz,sizeof(int));
	if (volume->data_type == ByteData) {
		bvol = (u_char*)volume->data_ptr;
		for (z=0; z<sz; z++) {
			sum_line = sum+z*sy;
			bplane = bvol + sx*sy*z;
			for (y=0; y<sy; y++) {
				bline = bplane+sx*y;
				for (x=x0; x<x1; x++) sum_line[y] += bline[x];
			}
		}
	} else {
		svol = (short*)volume->data_ptr;
		for (z=0; z<sz; z++) {
			sum_line = sum+z*sy;
			splane = svol + sx*sy*z;
			for (y=0; y<sy; y++) {
				sline = splane+sx*y;
				for (x=x0; x<x1; x++) sum_line[y] += sline[x];
			}
		}
	}
	return sum;
}

int *y_projection(volume,y0,y1)
MatrixData* volume;
int y0, y1;
{
	int *sum, *sum_line;
	int x, y, z;
	int sx = volume->xdim, sy = volume->ydim, sz = volume->zdim;
	int width = y1-y0+1;

	sum = (int*)calloc(sx*sz,sizeof(int));
	if (volume->data_type == ByteData) {
		bvol = (u_char*)volume->data_ptr;
		for ( z=0; z<sz; z++) {
			sum_line = sum+sx*z;
			bplane = bvol + sx*sy*z;
			for (y=y0; y<y1; y++) {
				bline = bplane+sx*y;
				for (x=0; x<sx; x++) sum_line[x] += bline[x];
			}
		}
	} else {
		svol = (short*)volume->data_ptr;
		for ( z=0; z<sz; z++ ) {
			sum_line = sum+sx*z;
			splane = svol + sx*sy*z;
			for (y=y0; y<y1; y++) {
				sline = splane+sx*y;
				for (x=0; x<sx; x++) sum_line[x] += sline[x];
			}
		}
	}
	return sum;
}

int *z_projection(volume, z0 , z1 )
MatrixData* volume;
int z0, z1;
{
}

main(argc, argv)
int argc;
char **argv;
{
	MatrixFile *mptr1, *mptr2;
	MatrixData *proj_slice;
	MatrixData* volume;
	Image_subheader *imagesub;
	struct Matval matval;
	int low=0, high=-1, width=0;
	int i, i_min, i_max, i_scale, npixels, nblks, *sum;
	short *data;
	Main_header proto;
	char fname[256];
	char *in_spec=NULL, *out_spec=NULL;
	int c, verbose = 0, cubic=1, matnum=0;
	DimensionName orthogonal = Dimension_Undefined;
	extern char *optarg;

	while ((c = getopt (argc, argv, "i:o:tcsl:v")) != EOF) {
		switch (c) {
		case 'i' :
			in_spec	= optarg;
            break;
		case 'o' :
			out_spec	= optarg;
            break;
		case 'l' :
			sscanf(optarg,"%d,%d",&low, &high);
            break;
		case 't' :
			orthogonal = Dimension_Z;
            break;
		case 'c' :
			orthogonal = Dimension_Y;
            break;
		case 's' :
			orthogonal = Dimension_X;
            break;
		case 'v' :
			verbose = 1;
			break;
		}
	}
	
	if (in_spec == NULL || out_spec==NULL) usage();
	if (orthogonal == Dimension_Undefined) usage();
	if (high<low) usage();
	width = high-low+1;
	matnum = mat_numcod(1,1,1,0,0);
	matspec( in_spec, fname, &matnum);
	mat_numdoc(matnum,&matval);
	mptr1 = matrix_open( fname, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
	if (!mptr1) crash( "%s: can't open file '%s'\n", argv[0], fname);
	if ( mptr1->dirlist->nmats == 0) crash("no matrix in %s\n",fname);
	if (matnum == 0) matnum = mptr1->dirlist->first->matnum;
	if ( (volume = load_volume(mptr1,matval.frame,cubic)) == NULL)
		crash( "%s: input matrix not found\n", in_spec);
	matspec( out_spec, fname, &matnum);	
	memcpy(&proto,mptr1->mhptr,sizeof(proto));
	proto.sw_version = V7;
	proto.plane_separation = volume->z_size*width;
	proto.num_planes = 1;
	proto.num_frames = 1;
	mptr2 = matrix_create( fname, MAT_OPEN_EXISTING, &proto);
	if (!mptr2) crash( "%s: can't open file '%s'\n", argv[0], fname);

	if (verbose) {
		fprintf(stderr,"input matrix : %s\n",in_spec);
		fprintf(stderr,"output matrix : %s\n",out_spec);
		if (proto.sw_version != mptr1->mhptr->sw_version) {
			fprintf(stderr,"converting version %d to version %d\n",
				mptr1->mhptr->sw_version, proto.sw_version); 
		}
	}
	proj_slice = (MatrixData*)calloc(sizeof(MatrixData),1);
	memcpy(proj_slice, volume, sizeof(MatrixData));
	proj_slice->shptr = (caddr_t)calloc(MatBLKSIZE,1);
	imagesub = (Image_subheader*)proj_slice->shptr;
	memcpy(imagesub,volume->shptr,sizeof(Image_subheader));
	proj_slice->zdim = 1;
	switch(orthogonal) {
	case Dimension_X :
		proj_slice->xdim = volume->ydim;
		proj_slice->ydim = volume->zdim;
		sum = x_projection(volume,low,high);
		break;
	case Dimension_Y :
		proj_slice->xdim = volume->xdim;
		proj_slice->ydim = volume->zdim;
		sum = y_projection(volume,low,high);
		break;
	case Dimension_Z :
		proj_slice->xdim = volume->xdim;
		proj_slice->ydim = volume->ydim;
		sum = z_projection(volume,low,high);
	default :
		usage();
	}
	imagesub->x_dimension = proj_slice->xdim;
	imagesub->y_dimension = proj_slice->ydim;
	imagesub->z_dimension = proj_slice->zdim;
	imagesub->z_pixel_size = proj_slice->z_size = proto.plane_separation;
	npixels = proj_slice->xdim*proj_slice->ydim;
	i_max = find_imax(sum,npixels);
	i_min = find_imin(sum,npixels);
	i_scale = (i_max+SHORT_MAX-1)/SHORT_MAX;
	nblks = (npixels*sizeof(short)+511)/512;
	proj_slice->data_size = nblks*MatBLKSIZE;
	data = (short*)calloc(MatBLKSIZE,nblks);
	proj_slice->data_ptr = (caddr_t)data;
	if (i_scale > 1) {
		for (i=0; i<npixels; i++) data[i] = sum[i]/i_scale;
		imagesub->scale_factor = imagesub->scale_factor*i_scale;
		imagesub->image_max = i_max/i_scale;
		imagesub->image_min = i_min/i_scale;
	} else {
		for (i=0; i<npixels; i++) data[i] = sum[i];
		imagesub->image_max = i_max;
		imagesub->image_min = i_min;
	}
/*	just integration; don't mean
	 imagesub->scale_factor = imagesub->scale_factor/width;	 */ 
	proj_slice->scale_factor = imagesub->scale_factor;
	proj_slice->data_max = proj_slice->scale_factor * imagesub->image_max;
	proj_slice->data_min = proj_slice->scale_factor * imagesub->image_min;
	matrix_write( mptr2, matnum, proj_slice);
	matrix_close( mptr1);
	matrix_close( mptr2);
}
