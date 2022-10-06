#include "matrix.h"

static  matrix_trim(matrix,sx1, sx2)
MatrixData *matrix;
int sx1, sx2;
{
    int 	trimmed=0, y, xdim, ydim;
    caddr_t  data_out, dest, src;
	int elem_size = 2;			/* assume short */

	if (matrix->data_type == ByteData) elem_size = 1;
	if (matrix->data_type == IeeeFloat) elem_size = 4;
	xdim = matrix->xdim; ydim = matrix->ydim;
	if (sx1 < xdim) {
		matrix->data_size = ((sx1*matrix->ydim*elem_size+511)/512)*512;
		data_out = (caddr_t)calloc(1, matrix->data_size);
		src = matrix->data_ptr + (xdim/2-sx1/2)*elem_size;
		dest = data_out;
		for (y=0; y<ydim; y++) {
			memcpy(dest,src,sx1*elem_size);
			src += xdim*elem_size;
			dest += sx1*elem_size;
		}
		xdim = matrix->xdim = sx1;
		free(matrix->data_ptr);
		matrix->data_ptr = (caddr_t)data_out;
		trimmed = 1;
	}
	if (sx2 < ydim) {
		src = matrix->data_ptr + (ydim/2-sx2/2)*xdim*elem_size;
		matrix->data_size = ((matrix->xdim*sx2*elem_size+511)/512)*512;
		data_out = (caddr_t)calloc(1, matrix->data_size);
		memcpy(data_out, src, sx2*xdim*elem_size);
		matrix->ydim = sx2;
		free(matrix->data_ptr);
		matrix->data_ptr = (caddr_t)data_out;
		trimmed = 1;
	}
	return trimmed;
}

static void matrix_short(matrix)
MatrixData *matrix;
{
	short *sdata;
	float scf, fmin, fmax, *fdata;
	int i, nvoxels;

	nvoxels = matrix->xdim*matrix->ydim*matrix->zdim;
	matrix->data_size = ((nvoxels*sizeof(short)+511)/512)*512;
	sdata = (short*)calloc(1,matrix->data_size);
	fdata = (float*)matrix->data_ptr;
	fmax = fmin = fdata[0];
	for (i=1; i<nvoxels; i++) {
		if (fmax < fdata[i]) fmax = fdata[i];
		else if (fmin > fdata[i]) fmin = fdata[i];
	}
	scf = fmax/32767;
	matrix->data_min = fmin;
	matrix->data_max = fmax;
	for (i=0; i<nvoxels; i++) sdata[i] = (int)(fdata[i]/scf);
	free(matrix->data_ptr);
	matrix->scale_factor = scf;
	matrix->data_ptr = (caddr_t)sdata;
}

static void matrix_float(matrix)
MatrixData *matrix;
{
	u_char *bdata;
	short *sdata;
	float scf, *fdata;
	int i, nvoxels;

	nvoxels = matrix->xdim*matrix->ydim*matrix->zdim;
	fdata = (float*)calloc(nvoxels,sizeof(float));
	scf = matrix->scale_factor;
	if (matrix->data_type == ByteData) {
		bdata = (u_char*)matrix->data_ptr;
		for (i=0; i<nvoxels; i++) fdata[i] = bdata[i]*scf;
	} else {			/* assume short */
		sdata = (short*)matrix->data_ptr;
		for (i=0; i<nvoxels; i++) fdata[i] = sdata[i]*scf;
	}
	free(matrix->data_ptr);
	matrix->scale_factor = 1;
	matrix->data_ptr = (caddr_t)fdata;
}

void matrix_interp(matrix, sx1, sx2)		/* assume float data */
MatrixData *matrix;
int sx1, sx2;
{
	int i1, i2, j, k;
	int x1a, x1b, x2a, x2b;
	int sx1a, sx2a;
	float x1, x2; 
	float y1,y2,y3,y4;
	float t, u;
	float *ya, *y;
	float dx1 = ((float)matrix->xdim)/sx1;
	float dx2 = ((float)matrix->ydim)/sx2;
	float scf = dx1*dx2; /* keeps scan integral (statistics) constant*/

	sx1a = matrix->xdim; sx2a = matrix->ydim;
	y = (float*)calloc(sx1*sx2, sizeof(float));
	ya = (float*)matrix->data_ptr;
/* 
	Bilinear interpolation from Numerical Recipes 
*/
	for (i1=0; i1<sx1; i1++) {
		x1 = dx1 * i1;
		j = (int) x1;
		for (i2=0; i2<sx2; i2++) {
			x2 = dx2 * i2;
			k = (int)x2;
			if (j+1 < sx1a && k+1<sx2a) {
				y1 = ya[k*sx1a + j];
				y2 = ya[k*sx1a + (j+1)];
				y3 = ya[(k+1)*sx1a + (j+1)];
				y4 = ya[(k+1)*sx1a + j];
				t = (x1 -j);
				u = (x2 - k);
				y[i2*sx1+i1] = scf*((1-t)*(1-u)*y1 + t*(1-u)*y2 + t*u*y3 +
					(1-t)*u*y4);
			} else y[i2*sx1+i1] = ya[k*sx1a + j];
			
		}
	}
	matrix->xdim = sx1; matrix->ydim = sx2;
	matrix->pixel_size *= dx1;
	matrix->y_size *= dx2;
	matrix->data_ptr = (caddr_t)y;
	free(ya);
}


static usage() {
	fprintf(stderr,
		"usage: image_resize -i matspec -s scale -o matspec [-V version -v]\n");
	fprintf(stderr,"version is either 70 or 6 (default = 70)\n");
	fprintf(stderr,"-v set verbose mode on ( default is off)\n");
	exit(1);
}
	
main( argc, argv)
  int argc;
  char **argv;
{
	MatrixFile *mptr1, *mptr2;
	MatrixData *matrix, *slice;
	Main_header proto;
	Image_subheader* imagesub;
	char fname[256];
	char *in_spec=NULL, *out_spec=NULL;
	int c, verbose = 0, version=V7, matnum=0;
	int frame, plane,  i, npixels, slice_blks, slice_matnum;
	int trim=0, elem_size =2, offset = 0;
	float scale = -1;
	short *sdata;
	u_char *bdata;
	struct Matval val;
	extern char *optarg;

	while ((c = getopt (argc, argv, "i:o:V:s:v")) != EOF) {
		switch (c) {
		case 'i' :
			in_spec	= optarg;
            break;
		case 'o' :
			out_spec	= optarg;
            break;
		case 'V' :
			sscanf(optarg,"%d",&version);
            break;
		case 's' :
			sscanf(optarg,"%f",&scale);
            break;
		case 'v' :
			verbose = 1;
			break;
		}
	}
	
	if (in_spec == NULL || out_spec==NULL || scale<=0) usage();
	matspec( in_spec, fname, &matnum);
	mptr1 = matrix_open( fname, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
	if (!mptr1) crash( "%s: can't open file '%s'\n", argv[0], fname);
	if ( mptr1->dirlist->nmats == 0) crash("no matrix in %s\n",fname);
	if (matnum == 0) matnum = mptr1->dirlist->first->matnum;
	matrix = matrix_read( mptr1, matnum, MAT_SUB_HEADER);
	if (!matrix)
	  crash( "%s: matrix not found\n", in_spec);
	if (matrix->zdim < 2) slice = matrix_read(mptr1,matnum, GENERIC);

	imagesub = (Image_subheader*)calloc(1,512);
	memcpy(imagesub, matrix->shptr,sizeof(Image_subheader));
	imagesub->z_dimension = 1;
	matspec( out_spec, fname, &matnum);
	mat_numdoc(matnum, &val);
	memcpy(&proto,mptr1->mhptr,sizeof(Main_header));
	proto.sw_version = version;
	if (version < V7) {
		if (proto.file_type != PetImage && proto.file_type != ByteVolume &&
		proto.file_type != PetVolume && proto.file_type != ByteImage &&
		proto.file_type != InterfileImage)
			crash ("version 6 : only images are supported \n");
		proto.file_type = PetImage;
	} else {
		if (proto.file_type == InterfileImage) {
			if (matrix->data_type == ByteData) proto.file_type = ByteVolume;
			else proto.file_type = PetVolume;
		}
	}
	proto.file_type = PetImage;
	mptr2 = matrix_create( fname, MAT_OPEN_EXISTING, &proto);
	if (!mptr2) crash( "%s: can't open file '%s'\n", argv[0], fname);

	if (verbose) {
		fprintf(stderr,"input matrix : %s\n",in_spec);
		fprintf(stderr,"output matrix : %s\n",out_spec);
		if (proto.sw_version != mptr1->mhptr->sw_version) {
			fprintf(stderr,"converting version %d to version %d\n",
				mptr1->mhptr->sw_version, proto.sw_version); 
		} else {
			fprintf(stderr,"input/output version : %d\n",proto.sw_version);
		}
	}

	npixels =  matrix->xdim*matrix->ydim;
	trim = (int)(matrix->xdim/scale);
	for (plane=1; plane<=matrix->zdim; plane++) {
	if (verbose) fprintf(stderr, "\nmatrix %d,%d,1,0,0 :\n",val.frame,plane);
		if (matrix->zdim > 1)
			slice = matrix_read_slice(mptr1, matrix, plane-1, 0);
	if (verbose) fprintf(stderr, "trimming from %d to %d ...\n",matrix->xdim, trim);
		matrix_trim(slice,trim,trim);
		matrix_float(slice);
	if (verbose) fprintf(stderr, "interpolating from %d to %d ... \n",trim,128);
		matrix_interp(slice,128,128);
		matrix_short(slice);
		slice->shptr = (caddr_t)imagesub;
		imagesub->x_dimension = slice->xdim;
		imagesub->y_dimension = slice->ydim;
		imagesub->z_dimension = slice->zdim;
		imagesub->image_max = (int)(slice->data_max/slice->scale_factor);
		imagesub->image_min = (int)(slice->data_min/slice->scale_factor);
		if (matrix->zdim>1) slice_matnum = mat_numcod(val.frame,plane,1,0,0);
		else slice_matnum = matnum;
		imagesub->x_pixel_size = slice->pixel_size;
		imagesub->y_pixel_size = slice->y_size;
		if (matrix_write(mptr2,slice_matnum,slice) < 0)
			matrix_perror(mptr2->fname);
	if (verbose) fprintf(stderr, "done\n");
		slice->shptr = NULL;
		free_matrix_data(slice);
	}
	matrix_close( mptr1);
	matrix_close( mptr2);
}
