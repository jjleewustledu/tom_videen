#include <stdio.h>
#include "load_volume.h"

static usage() {
	fprintf(stderr, "usage: volume_reduce -i in_matspec -o out_matspec [-r] [-c start_plane[,end_plane]]\n");
	fprintf(stderr, "-c start_plane[,end_plane] : clips the input volume in the axial direction\n");
	fprintf(stderr, "\tAssume HR961 axial FOV when end_plane not specified\n");
	exit(1);
}
static void bcompress2x2(u_char *image, int sx0, int sy0) {
    int i,j, sx1, sy1;
    u_char *p00, *p01, *p1;

    sx1 = sx0/2; sy1=sy0/2;
    p00 = image; p01 = image+sx0;
    p1 = image;
    for (j=0; j<sy1;j++) {
        for (i=0; i<sx1;i++)
            *p1++ = ((*p00++) + (*p00++) + (*p01++) + (*p01++))/4;
        p00 += sx0;
        p01 += sx0;
    }
}
static void scompress2x2(u_short *image, int sx0, int sy0) {
    int i,j, sx1, sy1;
    u_short *p00, *p01, *p1;

    sx1 = sx0/2; sy1=sy0/2;
    p00 = image; p01 = image+sx0;
    p1 = image;
    for (j=0; j<sy1;j++) {
        for (i=0; i<sx1;i++)
            *p1++ = ((*p00++) + (*p00++) + (*p01++) + (*p01++))/4;
        p00 += sx0;
        p01 += sx0;
    }
}

main( argc, argv)
  int argc;
  char **argv;
{	
	MatrixFile *mptr1, *mptr2;
	MatrixData *matrix;
	Main_header proto;
	Image_subheader *imh;
	char fname[256], *in_spec=NULL, *out_spec=NULL;
	int matnum1=0, matnum2=0;
	struct Matval matval;
	int c, cubic = 0, interpolate=0, verbose=0;
	int start_plane=0, end_plane=0, rebin=0;
	float default_fov = 47*0.3125;		/* ECAT 961 HR axial FOV in cm */
	int i, npixels, plane_size;
	caddr_t p0, p1;
	float q_scale = 1.0;
	extern char *optarg;

   while ((c = getopt (argc, argv, "c:i:o:rv")) != EOF) {
        switch (c) {
        case 'i' :
            in_spec = optarg;
            break;
        case 'o' :
            out_spec    = optarg;
            break;
		case 'c' :
			if (sscanf(optarg,"%d,%d",&start_plane,&end_plane) < 1) usage();
			break;
		case 'r' :
			rebin =1;
			break;
		case 'v' :
			verbose =1;
		}
	}


	if (in_spec==NULL || out_spec==NULL) usage();
	if (!matspec( in_spec, fname, &matnum1))
		matnum1 = mat_numcod(1,1,1,0,0);
	mptr1 = matrix_open( fname, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
	if (!mptr1)
	  crash( "%s: can't open file '%s'\n", argv[0], fname);

	mat_numdoc( matnum1, &matval);
	matrix = load_volume( mptr1, matval.frame,cubic,interpolate);
	if (!matrix)
	  crash( "%s: specified matrix not found\n", argv[0]);
	matrix->scale_factor *= q_scale;
	if (verbose) fprintf(stderr,"scale_factor*%g = %g\n",q_scale,matrix->scale_factor);
	matspec( out_spec, fname, &matnum2);
	if (matnum2 == 0) matnum2 = matnum1; 	/* use same specifications */
	proto = *mptr1->mhptr;
	proto.sw_version = 70;
 	proto.file_type = PetVolume;		/* volume mode */	
	proto.plane_separation = matrix->z_size;
	npixels = matrix->xdim*matrix->ydim;
	imh = (Image_subheader*)matrix->shptr;
	if (matrix->data_type == ByteData) plane_size = npixels;
	else plane_size = npixels*2; /* assume short */
	if (start_plane > 1) {
		if (end_plane <= start_plane) 
			end_plane = start_plane +
				(int)(0.5+default_fov/proto.plane_separation) -1;
		proto.num_planes = end_plane-start_plane+1;
if (verbose) fprintf(stderr,"axial clipping : %d,%d (%g mm)\n",start_plane,end_plane, proto.num_planes*matrix->z_size);
		p0 = matrix->data_ptr;
		p1 = p0 + (start_plane-1)*plane_size;
		for (i=start_plane; i<=end_plane && i<=matrix->zdim; i++) {
			memcpy(p0,p1,plane_size);
			p0 += plane_size;
			p1 += plane_size;
		}
		matrix->data_size = proto.num_planes*plane_size;
		if (end_plane > matrix->zdim)  {
if (verbose) fprintf(stderr,"adding %d blank planes\n",end_plane-matrix->zdim);
			p1 = matrix->data_ptr + matrix->zdim*plane_size;
			if (p1>p0) memset(p0,0, (int)(p1-p0));	/* clear allocated area */
			matrix->data_ptr = realloc(matrix->data_ptr,matrix->data_size);
		}
		matrix->zdim = imh->z_dimension = proto.num_planes;
	}
	if (rebin) {
		if (matrix->data_type == ByteData) {
			u_char* bdata = (u_char*)matrix->data_ptr;
			for (i=0; i<matrix->zdim; i++) {
				if (i>0) memcpy(bdata+i*npixels/4, bdata+i*npixels, npixels);
				bcompress2x2(bdata+i*npixels/4,matrix->xdim, matrix->ydim);
			}
		} else {
			u_short* sdata = (u_short*)matrix->data_ptr;
			for (i=0; i<matrix->zdim; i++) {
				if (i>0) memcpy(sdata+i*npixels/4, sdata+i*npixels,
					npixels*sizeof(u_short));
				scompress2x2(sdata+i*npixels/4,matrix->xdim, matrix->ydim);
			}
		}
		matrix->xdim /= 2;	imh->x_dimension /= 2;
		matrix->ydim /= 2;	imh->y_dimension /= 2;
		matrix->data_size /= 4;
		matrix->pixel_size *= 2; imh->x_pixel_size *= 2;
		matrix->y_size *= 2; imh->y_pixel_size *= 2;
	}
	if ((mptr2 = matrix_create( fname, MAT_OPEN_EXISTING, &proto)) != NULL) {
		matrix_write( mptr2, matnum2, matrix);
		matrix_close( mptr1);
		matrix_close( mptr2);
	} else matrix_perror(fname);
}
