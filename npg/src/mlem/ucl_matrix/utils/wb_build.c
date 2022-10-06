/* sccsid = "%W%  UCL-TOPO  %E%" */
#include "matrix.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
/*
#include "compute_decay.h"
suppose that images are decay corrected
*/


static int ref_time=0;		/* reference min scan start time */
static MatrixData *volume1, *volume2;
static int verbose = 1;
static int debug = 1;
typedef struct _FilePosition {
	char *filename;
	float position;
} FilePosition;

static usage() {
	fprintf(stderr,"usage : wb_build file1 ... filen wb_file\n");
	fprintf(stderr,"\tfile1 ... filen must be image or scan stored as short integers\n");
	fprintf(stderr,"\teg : wb_build p01572fd*.img p01572fdg.img to assemble\n");
	fprintf(stderr,"\tp01572fd1.img ... p01572fd11.img into p01572fdg.img\n");
	fprintf(stderr,"\tinterframe decay correction is always applied except when\n");
	fprintf(stderr,"\t\tWB_BUILD_NODECAY environment variable is set to 1\n");
	fprintf(stderr,"\tThe program does nothing if the output file exists\n");
	exit(1);
}

#ifdef __STDC__
static float file_decay(MatrixFile* file)
#else
static float file_decay(file)
MatrixFile* file;
#endif
{
	float decay_factor=1.0, lamda, halflife, t;

	t = file->mhptr->scan_start_time - ref_time;
	halflife = file->mhptr->isotope_halflife;
	if (halflife > 0) {
		lamda = log(2.0)/halflife;
		decay_factor = exp(lamda*t);
	 }
	if (verbose) fprintf (stderr, "halflife, t times = %g,%g\n",halflife,t);
	if (verbose) fprintf (stderr, "Decay Correction = %g\n", decay_factor);
}

static wb_copy(file_out,pl_out,file_in,pl_in,decay)
MatrixFile *file_in, *file_out;
int pl_out, pl_in;
float decay;
{
	Scan_subheader *sh;
	Image_subheader* imh;
	MatrixData *matrix = matrix_read_slice(file_in, volume1, pl_in-1, GENERIC);
	if (!matrix) matrix_perror(file_in->fname);
	matrix->scale_factor *= decay;
	if (file_out->mhptr->file_type == PetImage) {
		imh = (Image_subheader*)matrix->shptr;
		imh->decay_corr_fctr = decay;
		imh->scale_factor *= decay;
		imh->frame_start_time = ref_time;
	} else {
		sh = (Scan_subheader*)matrix->shptr;
		sh->frame_start_time = ref_time;
		sh->scale_factor *= decay;
	}
	matrix_write( file_out, mat_numcod(1,pl_out,1,0,0), matrix);
	if (debug) printf("copy %s,1,%d,1 (decay corrected : %g)  ===> %s,1,%d,1 done\n",
		file_in->fname, pl_in, decay, file_out->fname,pl_out);
	free_matrix_data(matrix);
}

static wb_merge( file_out, pl_out, file_in1,pl1,w1,file_in2,pl2,w2)
MatrixFile *file_in1, *file_in2, *file_out;
int pl_out, pl1,pl2;
float w1, w2;
{
	MatrixData *m1, *m2;
	Image_subheader *imh;
	Scan_subheader *sh;
	
	float *f_data, f_min, f_max;
	float scale1,scale2, scale;
	short *p1, *p2;
	int i=0, size=0;

	m1 = matrix_read_slice( file_in1, volume1,pl1-1, GENERIC);
	if (!m1) matrix_perror(file_in1->fname);
	m2 = matrix_read_slice( file_in2, volume2, pl2-1, GENERIC);
	if (!m2) matrix_perror(file_in2->fname);
	size = m1->xdim*m1->ydim;
	f_data = (float*)calloc(size,sizeof(float));
	p1 = (short*)m1->data_ptr; p2 = (short*)m2->data_ptr;
	scale1 = m1->scale_factor; scale2 = m2->scale_factor;
	f_min = f_max = f_data[0] = (w1*scale1*p1[0])+(w2*scale2*p2[0]);
	for (i=1; i<size; i++) {
		f_data[i] = (w1*scale1*p1[i])+(w2*scale2*p2[i]);
		if (f_max < f_data[i]) f_max = f_data[i];
		if (f_min > f_data[i]) f_min = f_data[i];
	}
	scale =  f_max/32767;
	if (debug)
		printf("merge %g * %s,1,%d,1 + %g * %s,1,%d,1  ===> %s,1,%d,1 done\n",
		w1,file_in1->fname,pl1,w2,file_in2->fname,pl2,file_out->fname,pl_out);
	if (debug) printf("data_max : %g,%g ===> %g\n",m1->data_max,m2->data_max,f_max);
	if (debug) printf("scale factors : %g,%g ===> %g\n",scale1,scale2,scale);
	m1->data_max = f_max;
	m1->data_min = f_min;
	for (i=0; i<size; i++) p1[i] = (short)(f_data[i]/scale);
	m1->scale_factor = f_max/32767;
	if (file_out->mhptr->file_type == PetImage) {
		imh = (Image_subheader*)m1->shptr;
		imh->scale_factor = m1->scale_factor;
		imh->frame_start_time = ref_time;
	} else {
		sh = (Scan_subheader*)m1->shptr;
		sh->scale_factor = m1->scale_factor;
		sh->frame_start_time = ref_time;
	}
	matrix_write( file_out, mat_numcod(1,pl_out,1,0,0), m1);
	free_matrix_data(m1); free_matrix_data(m2);
	free(f_data);
}

int main(argc,argv)
int argc;
char **argv;
{
	FilePosition tmp, *file_positions = NULL;
	MatrixFile *file_in1, *file_in2, *file_out;
	Main_header mhead, *mh;
	int i=0, j=0, count=0, sorted=0;
	int plane = 1;
	int file_time=0;		/* min scan start time */
	int apply_decay = 1;
	char* s;
	float decay_1=1.0, decay_2=1.0;
	struct tm *tm;
	
	if ((s=getenv("WB_BUILD_NODECAY")) != 0 && strcmp(s,"0")>0) apply_decay=0;
if (s!=NULL) printf("WB_BUILD_NODECAY=%s\n",s);
	if (apply_decay) fprintf(stderr,"interframe decay applied\n");
	else fprintf(stderr,"interframe decay IS NOT applied\n");
	if (argc < 3) usage();
	count = argc-2;
	file_positions = (FilePosition*)calloc(sizeof(FilePosition),count);
	for (i=1; i<=count; i++) file_positions[i-1].filename = argv[i];
	for (i=0; i<count; i++) {
		file_in1 = matrix_open(file_positions[i].filename,
			MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
		if (file_in1 == NULL) matrix_perror(file_positions[i].filename);
		mh = file_in1->mhptr;
		file_time = mh->scan_start_time;
		if (i==0) ref_time = file_time;
		else if (ref_time > file_time) ref_time = file_time;
		file_positions[i].position = mh->init_bed_position;
		matrix_close(file_in1);
	}
	if (verbose) {
		tm = localtime(&ref_time);
		fprintf(stderr,"min scan start time = %d:%d:%d\n",
			tm->tm_hour, tm->tm_min, tm->tm_sec);
	}
/* buble sort */
	while (!sorted) {
		sorted = 1;
		for (i=1; i<count; i++) {
			if (file_positions[i].position < file_positions[i-1].position) {
				sorted = 0;
				tmp = file_positions[i];
				file_positions[i] = file_positions[i-1];
				file_positions[i-1] = tmp;
			}
		}
	}

	for (i=0; i<count; i++) 
		printf("%s : %g\n",file_positions[i].filename,
			file_positions[i].position);

	if (access(argv[argc-1],F_OK) == 0) {
		fprintf(stderr, "%s already exists\7\n",argv[argc-1]);
		usage();
	}
	file_in1 = matrix_open(file_positions[count-1].filename, MAT_READ_ONLY,
		MAT_UNKNOWN_FTYPE);
	memcpy(&mhead,file_in1->mhptr,sizeof(Main_header));
	mhead.num_planes = 42+((5+37)*(count-1))+5;
	switch(mhead.file_type) {
	case PetImage:
	case PetVolume:
		mhead.file_type = PetImage;
		break;
	case Sinogram:
	case Short3dSinogram:
		mhead.file_type = Sinogram;
		break;
	default:
		crash("input must be Image or Sinogram stored as short integer\n");
	}
	file_out = matrix_create(argv[argc-1],MAT_OPEN_EXISTING, &mhead);
	if (!file_out) matrix_perror(argv[argc-1]);

	file_in1 = matrix_open(file_positions[0].filename,
		MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
	volume1 = matrix_read(file_in1, mat_numcod(1,1,1,0,0), MAT_SUB_HEADER);
	if (apply_decay) decay_1 = file_decay(file_in1);
	for (j=1; j<=42; j++) wb_copy(file_out,plane++,file_in1,j,decay_1);
	free_matrix_data(volume1);
	matrix_close(file_in1);

	for (i=1; i<count; i++) {
		file_in1 = matrix_open(file_positions[i-1].filename,
			MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
		volume1 = matrix_read(file_in1, mat_numcod(1,1,1,0,0), MAT_SUB_HEADER);
		if (apply_decay) decay_1 = file_decay(file_in1);
		file_in2 = matrix_open(file_positions[i].filename,
			MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
		volume2 = matrix_read(file_in2, mat_numcod(1,1,1,0,0), MAT_SUB_HEADER);
		if (apply_decay) decay_2 = file_decay(file_in2);

		wb_merge( file_out, plane++, file_in1,43,decay_1*.832,file_in2,1,
			decay_2*.179);
		wb_merge( file_out, plane++, file_in1,44,decay_1*.698,file_in2,2,
			decay_2*.379);
		wb_merge( file_out, plane++, file_in1,45,decay_1*.533,file_in2,3,
			decay_2*.568);
		wb_merge( file_out, plane++, file_in1,46,decay_1*.341,file_in2,4,
			decay_2*.729);
		wb_merge( file_out, plane++, file_in1,47,decay_1*.136,file_in2,5,
			decay_2*.864);

		free_matrix_data(volume1); volume1 = volume2;
		for (j=6; j<=42; j++) wb_copy(file_out,plane++,file_in2,j,decay_2);

		free_matrix_data(volume1);
		matrix_close(file_in1); matrix_close(file_in2);
	}

	file_in1 = matrix_open(file_positions[count-1].filename,
		MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
	volume1 = matrix_read(file_in1, mat_numcod(1,1,1,0,0), MAT_SUB_HEADER);
	if (apply_decay) decay_1 = file_decay(file_in1);
	for (j=43; j<=47; j++) wb_copy(file_out,plane++,file_in1,j,decay_1);
	free_matrix_data(volume1);
	matrix_close(file_in1);
}
