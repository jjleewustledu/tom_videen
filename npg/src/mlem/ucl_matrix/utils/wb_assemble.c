#include "matrix.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#define MAX_BEDS 20

typedef struct _BedPosition {
	int bed;
	float position;
	struct MatDir matdir;
	MatrixData* matrix;
} BedPosition;

static MatrixFile *file_in=NULL;
static MatrixFile *file_out=NULL;
static int verbose = 0;

static usage() {
	fprintf(stderr,"usage : wb_assemble -i multi_bed_file -o wb_file [-p bed_pos_file] [-v -x bi]\n");
	fprintf(stderr,"The output file is overwritten if exists\n");
	exit(1);
}

static MatrixData *blank() {
	static MatrixData *ret=NULL;
	if (ret == NULL) {
		ret = matrix_read(file_in,file_in->dirlist->first->matnum,
			MAT_SUB_HEADER);
		ret->data_size = ret->xdim*ret->ydim*sizeof(short);
		ret->data_ptr = (caddr_t)calloc(ret->data_size,1);
	}
	return ret;
}
	

#ifdef __STDC__
wb_copy(int pl_out, BedPosition *bed, int pl_in)
#else
wb_copy(pl_out,bed,pl_in)
int pl_out, pl_in;
BedPosition *bed;
#endif
{
	int size,slice_blksize,strtblk, endblk;
	struct MatDir *matdir;
	MatrixData *matrix;

	matdir = &bed->matdir;
	if (!bed->matrix)
		bed->matrix = matrix_read(file_in, matdir->matnum, MAT_SUB_HEADER);
	if (!bed->matrix) {		/* assume 128x128 short int image */
		fprintf(stderr,"Warning :  Plane  %s,1,%d,1,0,%d missing\n",
			file_in->fname, pl_in, bed->bed);
		matrix_write(file_out,mat_numcod(1,pl_out,1,0,0),blank());
	} else {
		size = bed->matrix->xdim*bed->matrix->ydim;
		slice_blksize = size/(MatBLKSIZE/2);
		bed->matrix->data_size = size*2;
		bed->matrix->zdim = 1;
		strtblk = matdir->strtblk+((pl_in-1)*slice_blksize);
 		endblk = strtblk + slice_blksize;
		bed->matrix->data_ptr = calloc(bed->matrix->data_size,sizeof(short));
		mat_read_matrix_data (file_in->fptr, file_in->mhptr, (strtblk + 1),
			(endblk - strtblk), (short*)bed->matrix->data_ptr);
		if (verbose) printf("copy %s,1,%d,1,0,%d	===> %s,1,%d,1 done\n",
			file_in->fname, pl_in, bed->bed, file_out->fname,pl_out);
		matrix_write(file_out,mat_numcod(1,pl_out,1,0,0),bed->matrix);
	}
}

#ifdef __osf__
wb_merge(int pl_out, BedPosition *bed_1, int pl1, float w1,
BedPosition *bed_2, int pl2, float w2)
#else
wb_merge(pl_out, bed_1,pl1,w1,bed_2,pl2,w2)
BedPosition *bed_1, *bed_2;
int pl_out, pl1,pl2;
float w1, w2;
#endif
{
	MatrixData *m1, *m2;
	float *f_data, f_min, f_max;
	int i, size,slice_blksize,strtblk, endblk;
	struct MatDir *matdir_1, *matdir_2;
	float scale1,scale2, scale;
	short *p1, *p2;

	matdir_1 = &bed_1->matdir;
	matdir_2 = &bed_2->matdir;
	if (!bed_1->matrix) {
		bed_1->matrix = matrix_read(file_in, matdir_1->matnum, MAT_SUB_HEADER);
		if (!bed_1->matrix) {
			fprintf(stderr,"Warning :  Plane  %s,1,%d,1,0,%d missing\n",
				file_in->fname, pl1, bed_1->bed);
		}
	}
	if (!bed_2->matrix) {
		bed_2->matrix = matrix_read(file_in, matdir_2->matnum, MAT_SUB_HEADER);
		if (!bed_2->matrix) {
			fprintf(stderr,"Warning :  Plane  %s,1,%d,1,0,%d missing\n",
			file_in->fname, pl2, bed_2->bed);
		}
	}

	if (bed_1->matrix==NULL && bed_2->matrix==NULL) {
		matrix_write( file_out,	mat_numcod(1,pl_out,1,0,0),blank());
		return;
	}
	if (bed_1->matrix==NULL) {		/* bed_2->matrix != NULL */
		wb_copy(pl_out,bed_2,pl2);
		return;
	}
	if (bed_2->matrix==NULL) {		/* bed_1->matrix != NULL */
		wb_copy(pl_out,bed_1,pl1);
		return;
	}
								/* bed_1->matrix!=NULL && bed_2->matrix!=NULL */
	m1 = bed_1->matrix; m2 = bed_2->matrix;
	size = m1->xdim*m1->ydim;
	f_data = (float*)calloc(size,sizeof(float));
	if (!m1->data_ptr) m1->data_ptr = calloc(size,sizeof(short));
	if (!m2->data_ptr) m2->data_ptr = calloc(size,sizeof(short));
	p1 = (short*)m1->data_ptr; p2 = (short*)m2->data_ptr;
	slice_blksize = size/(MatBLKSIZE/2);
	strtblk = matdir_1->strtblk+((pl1-1)*slice_blksize);
 	endblk = strtblk + slice_blksize;
	mat_read_matrix_data (file_in->fptr,file_in->mhptr,  (strtblk + 1), (endblk - strtblk), p1);
	strtblk = matdir_2->strtblk+((pl2-1)*slice_blksize);
 	endblk = strtblk + slice_blksize;
	mat_read_matrix_data (file_in->fptr, file_in->mhptr,(strtblk + 1), (endblk - strtblk), p2);

	scale1 = m1->scale_factor; scale2 = m2->scale_factor;
	f_min = f_max = f_data[0] = (w1*scale1*p1[0])+(w2*scale2*p2[0]);
	for (i=1; i<size; i++) {
		f_data[i] = (w1*scale1*p1[i])+(w2*scale2*p2[i]);
		if (f_max < f_data[i]) f_max = f_data[i];
		if (f_min > f_data[i]) f_min = f_data[i];
	}
	scale = f_max/32767;
	m1->data_max = f_max;
	m1->data_min = f_min;
	m1->zdim = 1;
	m1->data_size = size*2;
	m1->matnum = mat_numcod(1,pl_out,1,0,0);
	((Image_subheader*)m1->shptr)->scale_factor = scale;
	if (scale> 0) for (i=0; i<size; i++) p1[i] = (short)(f_data[i]/scale);
	matrix_write( file_out, m1->matnum,m1);
	free(f_data);
	if (verbose) printf("merge %g * 1,%d,1,0,%d + %g * %1,%d,1,0,%d  ===> %s,1,%d,1 done\n",
		w1,pl1,bed_1->bed,w2,pl2,bed_2->bed,file_out->fname,pl_out);
}

int main(argc,argv)
int argc;
char **argv;
{
	BedPosition tmp, *bed_positions = NULL, *bed_1, *bed_2;
	Main_header mhead, *mh;
	int i=0, j=0, count=0, sorted=0;
	int plane = 1;
	float bed_pos0, *bed_offsets;
	char *x_beds;
	int matnum;
	char in_name[128], out_name[128], bed_pos_file[128];
	struct MatDir matdir;
	FILE *fp;
	extern int optind, opterr;
	extern char *optarg;
	
	in_name[0] = '\0'; out_name[0] = '\0'; bed_pos_file[0] = '\0';
	x_beds = calloc(MAX_BEDS,1);
	while ((i = getopt (argc, argv, "i:o:x:p:v")) != EOF) {
	switch(i) {
	case 'v' : verbose = 1;
		break;
	case 'i' :	/* input file */
		  strncpy(in_name,optarg,127); in_name[127] = '\0';
		  break;
	 case 'o' :		/* out file */
		  strncpy(out_name,optarg,127); out_name[127] = '\0';
		  break;
	case 'p' :		/* bed_pos file */
		  strncpy(bed_pos_file,optarg,127); bed_pos_file[127] = '\0';
		  break;
	case 'x':
		j = atoi(optarg);
		if (j<MAX_BEDS) x_beds[j] = '1';
		break;
	case '?' : usage();
		  break;
	 }
	}
	if (!strlen(in_name) || !strlen(out_name)) usage();
	if (strcmp(in_name,out_name) == 0) {
		printf("input and output should differ\n");
		exit(1);
	}
	if (verbose) printf("input file : %s\noutputfile : %s\n",in_name,out_name);
	file_in = matrix_open(in_name, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
	if (file_in == NULL) matrix_perror(in_name);
	count =  file_in->mhptr->num_bed_pos+1;
	if (count < 2) {
		fprintf(stderr,"%s is not a multi bed position file\n");
		exit(1);
	}
	bed_positions = (BedPosition*)calloc(sizeof(BedPosition),count);
	bed_positions = (BedPosition*)calloc(sizeof(BedPosition),count);
	 bed_pos0 = file_in->mhptr->init_bed_position;
	 bed_offsets = file_in->mhptr->bed_offset;
	if (bed_pos_file[0] && (fp=fopen(bed_pos_file,"r"))!= NULL) {
		  for (i=0;i<count; i++)
				if (fscanf(fp,"%g",bed_offsets+i) != 1) break;
		  fclose(fp);
	 } else {		  /* correct storage nonsense */
/* problem fixed since Jun-1995 release 
		  for (i=count-1; i >= 2; i--) bed_offsets[i] = bed_offsets[i-2];
		  bed_offsets[1] = bed_offsets[2]/2;
		  bed_offsets[0] = 0;
*/
	 }
	for (i=0, j=0; i<count; i++) {
		if (x_beds[i]) continue;
		matnum = mat_numcod(1,1,1,0,i);
		if (matrix_find(file_in,matnum,&matdir) < 0 ) {
			fprintf(stderr,"matrix 1,1,1,0,%d not found\n",i);
		}
		bed_positions[j].bed = i;
		bed_positions[j].matdir = matdir;
		if (i>0) bed_positions[j].position = bed_pos0+bed_offsets[i-1];
		else  bed_positions[j].position = bed_pos0;
		j++;
	 }
	count = j;

/* buble sort */
	while (!sorted) {
		sorted = 1;
		for (i=1; i<count; i++) {
			if (bed_positions[i].position < bed_positions[i-1].position) {
				sorted = 0;
				tmp = bed_positions[i];
				bed_positions[i] = bed_positions[i-1];
				bed_positions[i-1] = tmp;
			}
		}
	}

	if (verbose) for (i=0; i<count; i++) 
		printf("%d : %g\n",bed_positions[i].bed, bed_positions[i].position);

/*	output file overwrite is checked at executive level
	if (access(out_name,F_OK) == 0) {
		fprintf(stderr, "Warning: %s is overwritten\7\n",out_name);
		usage();
	}
*/
	memcpy(&mhead,file_in->mhptr,sizeof(Main_header));
	mhead.num_planes = 42+((5+37)*(count-1))+5;
	for (i=0;i<mhead.num_bed_pos;i++) mhead.bed_offset[i] = 0.0;
	mhead.num_bed_pos = 1;
	mhead.init_bed_position = bed_positions[0].position;
	file_out = matrix_create(out_name,MAT_OPEN_EXISTING, &mhead);
	if (file_out == NULL) matrix_perror(out_name);

	bed_1 = bed_positions;
	for (j=1; j<=42; j++) wb_copy(plane++,bed_1,j);
	for (i=1; i<count; i++) {
		bed_1 = &bed_positions[i-1]; bed_2 = &bed_positions[i];

		wb_merge( plane++, bed_1,43, .832, bed_2, 1, .179);
		wb_merge( plane++, bed_1,44, .698, bed_2 ,2, .379);
		wb_merge( plane++, bed_1,45, .533, bed_2, 3, .568);
		wb_merge( plane++, bed_1,46, .341, bed_2, 4, .729);
		wb_merge( plane++, bed_1, 47, .136, bed_2, 5, .864);

		for (j=6; j<=42; j++) wb_copy(plane++,bed_2,j);
		free_matrix_data(bed_1->matrix);
		bed_1->matrix = 0;
	}

	bed_1 = &bed_positions[count-1];
	for (j=43; j<=47; j++) wb_copy(plane++,bed_1,j);
	free_matrix_data(bed_1->matrix);
	matrix_close(file_in);
	matrix_close(file_out);
}
