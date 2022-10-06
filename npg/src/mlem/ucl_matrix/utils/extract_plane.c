#include <time.h>
#include "matrix.h"

static usage() {
	fprintf(stderr,
		"usage: extract_plane in_file out_file plane_number [rebin]\n");
	exit(1);
}
	
static void matrix_rebin(slice)
MatrixData* slice;
{
	int i,j,  xdim, ydim, xdim2, ydim2;
	short *src, *dest;
	short *p, *p1, *p2;

	xdim2 = slice->xdim;
	ydim2 = slice->ydim;
	slice->xdim = xdim = xdim2/2;
	slice->ydim = ydim = ydim2/2;
	src = (short*)slice->data_ptr;
	dest = (short*)calloc(xdim*ydim, sizeof(short));
	p = dest;
	p1 = src; p2 = src+xdim2;
	for (j=0; j<ydim; j++) {
		for (i=1; i<xdim; i++) {
			*p = (p1[0]+p1[1]+p2[0]+p2[1])/4;
			p++; p1 += 2; p2 += 2;
		}
		p1 += xdim2; p2 += xdim2;
	}
	free(src);
	slice->data_ptr = (caddr_t)dest;
}

main( argc, argv)
  int argc;
  char **argv;
{
	MatrixFile *mptr;
	Main_header *mh;
	MatrixData *matrix=NULL, *slice=NULL;
	char *in_file=NULL, *out_file=NULL;
	char fname[256];
	FILE *fp=NULL, *fd_if=NULL;
	int i, plane=1, rebin=1;
	int num_slices = 0;
	int xdim, ydim, npixels;
	short *sdata=NULL;
	float *fdata=NULL;
	float scale_factor, fmin, fmax, pixel_size, global_min=0.0, global_max=0.0;
	struct Matval val;
	struct tm tm;
	MatDirNode *node;


	if (argc<4) usage();
	in_file = argv[1];
	out_file = argv[2];
	plane = atoi(argv[3]);
	if (argc>4) rebin = atoi(argv[4]);
	mptr = matrix_open(in_file, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
	if (!mptr) crash( "%s: can't open file '%s'\n", argv[0], in_file);
	fp = fopen(out_file, "w");
	if (fp==NULL) crash( "%s: can't open file '%s'\n", argv[0], out_file);
	if (mptr->dirlist->nmats == 0) crash("no matrix in %s\n",fname);
	node = mptr->dirlist->first;
	mh = mptr->mhptr;
/*
 * Create Interfile Format header file
 */

	sprintf(fname,"%s.h33",out_file);
	if ((fd_if = fopen (fname, "w")) == NULL) 
		crash( "%s: can't open file '%s'\n", argv[0], fname);
    fprintf (fd_if, "INTERFILE :=\n");
    fprintf (fd_if, "version of keys    := 3.3\n");
    fprintf (fd_if, "image modality := pet\n");
    fprintf (fd_if, "originating system := Siemens/CTI ECAT %d\n",
		mh->system_type);
    fprintf (fd_if, "conversion program := extract_plane\n");
    fprintf (fd_if, "program version    := 1.0\n");
    fprintf (fd_if, "original institution   := %s\n",
		mh->facility_name);
    fprintf (fd_if, "name of data file  := %s\n", out_file);
    fprintf (fd_if, "subject ID := %s\n", mh->patient_id);
	memcpy(&tm,localtime(&mh->scan_start_time), sizeof(tm));
    fprintf (fd_if, "study date := %-d:%-d:%-d\n",
		tm.tm_year+1900,tm.tm_mon,tm.tm_mday);
	
	while (node) {
		mat_numdoc(node->matnum, &val);
		if (val.plane == plane) 			/* single slice ? */
				slice = matrix_read(mptr, node->matnum, GENERIC);
		else { 								/* volume images */
			matrix = matrix_read(mptr, node->matnum, MAT_SUB_HEADER);
			if (matrix && (matrix->zdim >= plane))
				slice = matrix_read_slice(mptr, matrix, plane-1, 0);
		}
		if (slice != NULL) {
			for (i=0; i<rebin; i++) matrix_rebin(slice);
			xdim = slice->xdim; ydim = slice->ydim;
			npixels = xdim*ydim;
			sdata = (short*)slice->data_ptr;
			scale_factor = slice->scale_factor;
			if (fdata == NULL) 
				fdata = (float*)malloc(xdim*ydim*sizeof(float));
			for (i=0; i<npixels; i++) fdata[i] = scale_factor*sdata[i];
			pixel_size = slice->pixel_size;
			fmin = find_fmin(fdata,npixels);
			fmax = find_fmax(fdata,npixels);
			if (global_min < fmin) global_min = fmin;
			if (global_max < fmax) global_max = fmax;
#ifndef TYPE_SHORT
			fwrite(fdata,npixels,sizeof(float),fp);
#else
			fwrite(sdata,npixels,sizeof(short),fp);
#endif
			free_matrix_data(slice);
			num_slices++;
		}
		if (matrix) free_matrix_data(matrix);
		slice = matrix = NULL;
		node = node->next;
	}
	fclose(fp);
	if (num_slices) {
#ifndef TYPE_SHORT
		fprintf (fd_if, "number format  := short float\n");
    	fprintf (fd_if, "number of bytes per pixel  := 4\n");
    	fprintf (fd_if, "maximum pixel count := %g\n",global_max);
		fprintf (fd_if, ";minimum pixel count := %g\n",global_min);
#else
		fprintf (fd_if, "number format  := short int\n");
#ifndef sun
		fprintf (fd_if, "imagedata byte order := LITTLEENDIAN\n");
#endif
	 	fprintf (fd_if, "number of bytes per pixel  := 2\n");
		fprintf (fd_if, "number of dimensions := 3\n");
    	fprintf (fd_if, "matrix size [3]    := %d\n", num_slices);
#endif
		fprintf (fd_if, "matrix size [1]    := %d\n", xdim);
    	fprintf (fd_if, "matrix size [2]    := %d\n", ydim);
    	fprintf (fd_if, "total number of images := %d\n", num_slices);
    	fprintf (fd_if, "scaling factor (mm/pixel) [1]  := %f\n",
			10 * pixel_size);
    	fprintf (fd_if, "scaling factor (mm/pixel) [2]  := %f\n",
			10 * pixel_size);
	}
	fclose(fd_if);
	matrix_close(mptr);
}

