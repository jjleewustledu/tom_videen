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
static Main_header mhead;
static Attn_subheader attn_sh;


static usage() {
	fprintf(stderr,"usage : wb_append -i input_file -o wb_scan_file,specs [-m angular_mash] [-r radial_rebin] [-v]\n");
	fprintf(stderr,"-m angular_mash (default = 7) \n");
	fprintf(stderr,"-r radial_rebin (default = 2) \n");
	fprintf(stderr,"The output file is overwritten if exists\n");
	exit(1);
}


#ifdef __STDC__
wb_scan_copy(int pl_out, MatrixData* matrix, struct MatDir *matdir,
int pl_in, int pl_in_end, int mash, int rebin)
#else
wb_scan_copy(pl_out, matrix, matdir, pl_in, pl_in_end, mash,rebin)
int pl_out, pl_in, pl_in_end, mash, rebin;
MatrixData *matrix;
struct MatDir *matdir;
#endif
{
	int i, j, view, n_pl, count;
	int file_pos_in, file_pos_out, out_planar_size, planar_size;
	int buf_size, o_buf_size;
	float *i_buf, *o_buf;
	float *tmp;
	int num_projs, num_views, num_planes;
	int compress_factor, swap_flag = 0;

	if (ntohs(1) != 1) swap_flag = 1;
	compress_factor = mash*rebin;
	n_pl = pl_in_end - pl_in + 1;
	num_projs = matrix->xdim/rebin;
	num_views = matrix->ydim;
	num_planes = matrix->zdim;
	planar_size = matrix->xdim*num_planes;
	out_planar_size = num_projs*mhead.num_planes;
	buf_size = matrix->xdim*n_pl;
	o_buf_size = num_projs*n_pl;
	i_buf = (float*)calloc(buf_size,sizeof(float));
	o_buf = (float*)calloc(buf_size,sizeof(float));
	if (swap_flag) tmp = (float*)calloc(buf_size,sizeof(float));
	file_pos_in = 3*MatBLKSIZE + (pl_in-1)*matrix->xdim*sizeof(float);
	file_pos_out = 3*MatBLKSIZE + (pl_out-1)*num_projs*sizeof(float);
	for (view=0; view<num_views; view += mash) {
		if (view+mash >= num_views) break;		/* ignore trailing views */
		for (i=0; i<mash; i++) {
			if (fseek(file_in->fptr, file_pos_in, 0) == EOF) 
				crash("%s : fseek error\n",file_in->fname);
			if (fread(i_buf,sizeof(float),buf_size, file_in->fptr) != buf_size)
				crash("%s : fread error\n",file_in->fname);
			if (swap_flag) {
				swab(i_buf,tmp,buf_size*sizeof(float));
				swaw(tmp,i_buf,buf_size*sizeof(float)/2);
			}
			for (j=0; j<buf_size; j++) o_buf[j/rebin] += i_buf[j];
			file_pos_in += planar_size*sizeof(float);
		}
		if (fseek(file_out->fptr, file_pos_out, 0) == EOF) 
			crash("%s : fseek error\n",file_out->fname);
		if (swap_flag) {
			swab(o_buf, tmp, o_buf_size*sizeof(float));
			swaw(tmp,o_buf,o_buf_size*sizeof(float)/2);
		}
		if ( (count=fwrite(o_buf,sizeof(float),o_buf_size,file_out->fptr)) !=
			o_buf_size) crash("%s : fwrite error\n",file_out->fname);
		file_pos_out += out_planar_size*sizeof(float);
		memset(o_buf,0,buf_size*sizeof(float));
	}
	free(i_buf); free(o_buf);
	if (swap_flag) free(tmp);
}

int main(argc,argv)
int argc;
char **argv;
{
	BedPosition bed_1;
	Main_header *mh;
	MatrixData *matrix;
	int i=0, j=0, count=0, sorted=0;
	int plane = 1, mash=7, rebin=2;
	float bed_pos0, *bed_offsets;
	char *all_0;
	char *x_beds;
	int i_matnum, matnum, blkno, nblks, data_size;
	char in_name[128], out_spec[128], out_name[128];
	struct MatDir i_matdir, i_dir_entry;
	struct MatDir matdir, dir_entry;
	FILE *fp;
	extern int optind, opterr;
	extern char *optarg;
	
	in_name[0] = '\0'; out_spec[0] = '\0';
	x_beds = calloc(MAX_BEDS,1);
	while ((i = getopt (argc, argv, "i:o:x:p:m:r:v")) != EOF) {
	switch(i) {
	case 'v' : verbose = 1;
		break;
	case 'i' :	/* input file */
			strncpy(in_name,optarg,127); in_name[127] = '\0';
			break;
	 case 'o' :		/* out file */
			strncpy(out_spec,optarg,127); out_spec[127] = '\0';
			break;
	case 'm':
		sscanf(optarg,"%d",&mash);
		break;
	case 'r':
		sscanf(optarg,"%d",&rebin);
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
	file_in = matrix_open(in_name, MAT_READ_ONLY, AttenCor);
	if (file_in == NULL) matrix_perror(in_name);

/*	output file overwrite is checked at executive level
	if (access(out_name,F_OK) == 0) {
		fprintf(stderr, "Warning: %s is overwritten\7\n",out_name);
		usage();
	}
*/
	memcpy(&mhead,file_in->mhptr,sizeof(Main_header));
	mhead.num_bed_pos = 1;
	matspec(out_spec, out_name, &matnum); 
	file_out = matrix_create(out_name,MAT_OPEN_EXISTING, &mhead);
	if (!file_out) matrix_perror(out_name);

	matrix = matrix_read(file_in,mat_numcod(1,1,1,0,0),MAT_SUB_HEADER);
	if (!matrix) matrix_perror(file_in->fname);
	memcpy(&attn_sh,matrix->shptr,sizeof(Attn_subheader));
	attn_sh.num_z_elements = mhead.num_planes;
	attn_sh.num_angles = attn_sh.num_angles/mash;
	attn_sh.num_r_elements = attn_sh.num_r_elements/rebin;
	attn_sh.scale_factor = attn_sh.scale_factor*mash*rebin;
	attn_sh.x_resolution *= rebin;
	attn_sh.y_resolution *= mash;
	data_size = mhead.num_planes*attn_sh.num_angles*attn_sh.num_r_elements*
		sizeof(float);
	nblks = 2 + (data_size+511)/MatBLKSIZE;
	if (matrix_find(file_out, matnum, &matdir) == -1) {
		blkno = mat_enter(file_out->fptr, file_out->mhptr, matnum, nblks) ;
		dir_entry.matnum = matnum ;
		dir_entry.strtblk = blkno ;
		dir_entry.endblk = dir_entry.strtblk + nblks - 1 ;
		dir_entry.matstat = 1 ;
		insert_mdir(dir_entry, file_out->dirlist) ;
		matdir = dir_entry ;
	} else fprintf(stderr,"\7warning : existing matrix overwritten\n");
	all_0 = calloc(512,1);
	mat_write_attn_subheader(file_out->fptr, file_out->mhptr, matdir.strtblk,
		&attn_sh);
	if (fseek(file_out->fptr, (blkno+1)*MatBLKSIZE, 0) == EOF)
		crash("%s : fseek error\n", file_out->fname);
	for (i=2; i<=nblks; i++)
		if (fwrite( all_0, 1, 512, file_out->fptr) != 512)
			crash("%s : fwrite error\n", file_out->fname);
	free(all_0);
	wb_scan_copy(plane,matrix,&matdir,1,47,mash,rebin);
	free_matrix_data(matrix);
	matrix_close(file_in);
	matrix_close(file_out);
}
