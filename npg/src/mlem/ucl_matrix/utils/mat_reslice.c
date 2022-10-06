/* @(#)matcopy.c	1.4 7/10/92 */
/* 09-Nov-1995 : modified by sibomana@topo.ucl.ac.be */

#include "sys/types.h"
#include "load_volume.h"

#define SHORT_MAX 32767

typedef enum {
		Dimension_X, Dimension_Y, Dimension_Z, Dimension_Undefined
	} DimensionName;

static short *svol=NULL, *splane=NULL, *sline=NULL;
static u_char *bvol=NULL, *bplane=NULL, *bline=NULL;
static usage(name) 
char *name;
{
	fprintf(stderr,
		"usage: %s -i in_matspec -o out_matspec -t|-c|-s -l min,max [-v]\n",
		name);
	fprintf(stderr,"-t for transverse slices or \n");
	fprintf(stderr,"-c for coronal slices or \n");
	fprintf(stderr,"-s for sagittal slices or \n");
	fprintf(stderr,"-v set verbose mode on ( default is off)\n");
	exit(1);
}

int *x_slice(volume,slice,x)
MatrixData *volume, *slice;
int x;
{
	short *sdata, *sdest;
	u_char *bdata, *bdest;
	int  y,z;
	int sx = volume->xdim, sy = volume->ydim, sz = volume->zdim;

	if (volume->data_type == ByteData) {
		bdata = (u_char*)slice->data_ptr;
		bvol = (u_char*)volume->data_ptr;
		for (z=0; z<sz; z++) {
			bdest = bdata+z*sy;
			bplane = bvol + sx*sy*z;
			for (y=0; y<sy; y++) bdest[y] = bplane[sx*y+x];
		}
	} else {
		svol = (short*)volume->data_ptr;
		sdata = (short*) slice->data_ptr;
		for (z=0; z<sz; z++) {
			sdest = sdata+z*sy;
			splane = svol + sx*sy*z;
			for (y=0; y<sy; y++) sdest[y] = splane[sx*y+x];
		}
	}
}

int *y_slice(volume,slice,y)
MatrixData *volume, *slice;
int y;
{
	short *sdata, *sdest;
	u_char *bdata, *bdest;
	int x, z;
	int sx = volume->xdim, sy = volume->ydim, sz = volume->zdim;

	if (volume->data_type == ByteData) {
		bvol = (u_char*)volume->data_ptr;
		bdata = (u_char*)slice->data_ptr;
		for ( z=0; z<sz; z++) {
			bdest = bdata+z*sy;
			bplane = bvol + sx*sy*z;
			bline = bplane+sx*y;
			for (x=0; x<sx; x++) bdest[x] = bline[x];
		}
	} else {
		svol = (short*)volume->data_ptr;
		sdata = (short*)slice->data_ptr;
		for ( z=0; z<sz; z++ ) {
			sdest = sdata+sx*z;
			splane = svol + sx*sy*z;
			sline = splane+sx*y;
			for (x=0; x<sx; x++) sdest[x] = sline[x];
		}
	}
}

int *z_slice(volume, slice , z )
MatrixData *volume, *slice;
int z;
{
	short *sdata, *sdest;
	u_char *bdata, *bdest;
	int npixels = volume->xdim*volume->ydim;

	if (volume->data_type == ByteData) {
		bvol = (u_char*)volume->data_ptr;
		bplane = bvol + npixels*z;
		memcpy(bdata, bplane, npixels);
	} else {
		svol = (short*)volume->data_ptr;
		sdata = (short*)slice->data_ptr;
		splane = svol + npixels*z;
		memcpy(sdata, splane, npixels*2);
	}
}

main(argc, argv)
int argc;
char **argv;
{
	MatrixFile *mptr1, *mptr2;
	MatrixData *slice;
	MatrixData* volume;
	Image_subheader *imagesub;
	struct Matval mat;
	int low=0, high=-1, width=0;
	int i, i_min, i_max, i_scale, npixels, nblks, *sum;
	short *data;
	Main_header proto;
	char fname[256];
	char *in_spec=NULL, *out_spec=NULL;
	int c, verbose = 0, cubic=1, matnum=0, plane=0;
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
	
	if (in_spec == NULL || out_spec==NULL) usage(argv[0]);
	if (orthogonal == Dimension_Undefined) usage(argv[0]);
	if (high<low) usage(argv[0]);
	width = high-low+1;
	matnum = mat_numcod(1,1,1,0,0);
	matspec( in_spec, fname, &matnum);
	mat_numdoc(matnum,&mat);
	mptr1 = matrix_open( fname, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
	if (!mptr1) crash( "%s: can't open file '%s'\n", argv[0], fname);
	if ( mptr1->dirlist->nmats == 0) crash("no matrix in %s\n",fname);
	if (matnum == 0) matnum = mptr1->dirlist->first->matnum;
	if ( (volume = load_volume(mptr1,mat.frame,cubic)) == NULL)
		crash( "%s: input matrix not found\n", in_spec);
	mat_numdoc(matnum,&mat);
	matspec( out_spec, fname, &matnum);	
	memcpy(&proto,mptr1->mhptr,sizeof(proto));
	proto.sw_version = V7;
	proto.plane_separation = volume->z_size;
	proto.num_planes = width;
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
	slice = (MatrixData*)calloc(sizeof(MatrixData),1);
	memcpy(slice, volume, sizeof(MatrixData));
	slice->shptr = (caddr_t)calloc(MatBLKSIZE,1);
	imagesub = (Image_subheader*)slice->shptr;
	memcpy(imagesub,volume->shptr,sizeof(Image_subheader));
	slice->zdim = 1;
	switch(orthogonal) {
	case Dimension_X :
		slice->xdim = volume->ydim;
		slice->ydim = volume->zdim;
		break;
	case Dimension_Y :
		slice->xdim = volume->xdim;
		slice->ydim = volume->zdim;
		break;
	case Dimension_Z :
		slice->xdim = volume->xdim;
		slice->ydim = volume->ydim;
	default :
		usage(argv[0]);
	}
	imagesub->x_dimension = slice->xdim;
	imagesub->y_dimension = slice->ydim;
	imagesub->z_dimension = slice->zdim;
	imagesub->z_pixel_size = slice->z_size = proto.plane_separation;
	npixels = slice->xdim*slice->ydim;
	nblks = (npixels*sizeof(short)+511)/512;
	slice->data_size = nblks*MatBLKSIZE;
	data = (short*)calloc(MatBLKSIZE,nblks);
	slice->data_ptr = (caddr_t)data;
	for (plane=1; plane<=width; plane++) {
		switch(orthogonal) {
		case Dimension_X :
			x_slice(volume, slice, low+plane-1);
			break;
		case Dimension_Y :
			y_slice(volume, slice, low+plane-1);
			break;
		case Dimension_Z :
			sum = z_slice(volume, slice, low+plane-1);
			break;
		}
		imagesub->image_max = find_smax(data,npixels);
		imagesub->image_min = find_smin(data,npixels);
		slice->data_max = slice->scale_factor * imagesub->image_max;
		slice->data_min = slice->scale_factor * imagesub->image_min;
		matnum = mat_numcod(mat.frame,plane,mat.gate,mat.data,mat.bed);
		matrix_write(mptr2,matnum,slice);
	}
	matrix_close( mptr1);
	matrix_close( mptr2);
}
