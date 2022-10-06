#include "matrix.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#define MAX_BEDS 20
#define host_data_to_file file_data_to_host

typedef struct _BedPosition {
	int bed;
	float position;
	struct MatDir matdir;
	MatrixData* matrix;
} BedPosition;

static MatrixFile *file_in=NULL;
static MatrixFile *file_out=NULL;
static char in_name[128];
static char norm_name[128];
static char tmp_file[128];
static int verbose = 0;
static Main_header mhead;
static Scan3D_subheader *scan_sh;
static Attn_subheader atten_sh;

static usage() {
	fprintf(stderr,"usage : wb_scan_assemble -i multi_bed_file -o wb_scan_file [-p bed_pos_file] [-N norm2d_file] [-m angular_mash]  [-r distance_rebin] [-v -x bi]\n");
	fprintf(stderr,"-m angular_mash (default = 1) \n");
	fprintf(stderr,"-r radial compression (default = 1) \n");
	fprintf(stderr,"The output file is overwritten if exists\n");
	exit(1);
}

static apply_norm(bed_num)
int bed_num;
{
	static char cmd[256];
	matrix_close(file_in);
	if (access(tmp_file,F_OK) == 0) {	/* tmp_file already exists */
		unlink(tmp_file);
		sprintf(cmd,"applynorm -i %s,1,1,1,0,%d -n %s -o %s",
			in_name,bed_num,norm_name, tmp_file);
		if (verbose) printf("spawn : %s ...",cmd);
		system(cmd);
		if (verbose) printf("done\n");
		file_in = matrix_open(tmp_file, MAT_READ_ONLY, AttenCor);
		if (file_in == NULL) {
			matrix_perror(tmp_file);
			unlink(tmp_file);
			exit(1);
		}
	}
}

static matrix_float(matrix)
MatrixData *matrix;
{
	float scalef, *fdata;
	short *sdata;
	int i, np = matrix->xdim*matrix->ydim;

	matrix->data_type = IeeeFloat;
	fdata = (float*)calloc(np,sizeof(float));
	sdata = (short*)matrix->data_ptr;
	scalef = matrix->scale_factor;
	matrix->data_ptr = (caddr_t)fdata;
	for (i=0; i<np;i++) fdata[i] = scalef * sdata[i];
	matrix->scale_factor = 1.0;
	free(sdata);
}

static matrix_rebin(matrix, x_rebin, y_rebin,mean)
MatrixData *matrix;
int x_rebin, y_rebin, mean;
{
	int   i, j, k=0;
	float *scan_in, *scan_out;
	float *fp_in, *fp_out;
	float scf=1./(x_rebin*y_rebin);
	int nprojs = matrix->xdim, nviews = matrix->ydim;
	
	if (x_rebin<=1 && y_rebin<=1) return;
	scan_out = (float*)calloc(sizeof(float),nprojs*nviews);
/* integer x rebin */
	scan_in = (float*)matrix->data_ptr;
	fp_in = scan_in; fp_out = scan_out;
	if (x_rebin > 1 ) {
		for (i=0; i<nprojs*nviews; i += x_rebin, fp_out++) {
			for (j=0; j<x_rebin; j++) *fp_out += *fp_in++;
		}
		matrix->xdim /= x_rebin;
		nprojs = matrix->xdim;
	}
	
/* integer y rebin */
	if (y_rebin > 1 ) {
		if (x_rebin > 1 ) {						/* swap in and out only if x_rebin */
		    memcpy(scan_in,scan_out,nprojs*nviews*sizeof(float));
		    memset(scan_out,0,nprojs*nviews*sizeof(float));  		/* set to 0.0 */
		}
		fp_in = scan_in; fp_out = scan_out;
		for (i=0; i<nviews; i++) {
			fp_out = scan_out + nprojs*(i/y_rebin);
			for (j=0; j<nprojs; j++) fp_out[j] += *fp_in++;
		}
		matrix->ydim /= y_rebin;
		nviews = matrix->ydim;
	}
	free(scan_in);
	/* if mean value i.e do not add pixel statistics*/  
	if (mean) for (i=0; i<nprojs*nviews; i++) scan_out[i] *= scf;

	matrix->data_ptr = (caddr_t)scan_out;
	matrix->pixel_size *= x_rebin;
	matrix->y_size *= y_rebin;
}

#ifdef __STDC__
wb_scan_copy(int pl_out, BedPosition *bed, int pl_in, int pl_in_end,
int mash, int rebin)
#else
wb_scan_copy(pl_out,bed,pl_in, pl_in_end, mash,rebin)
int pl_out, pl_in, pl_in_end, mash, rebin;
BedPosition *bed;
#endif
{
	int i, j, count;
	int file_pos;
	int npixels, nblks;
	int mean=0;		/* add statistics when rebinning */
	struct MatDir *matdir;
	MatrixData *slice;
	float *fdata;
	int num_projs, num_views;

	matdir = &bed->matdir;
	if (!bed->matrix)
		bed->matrix = matrix_read(file_in, matdir->matnum, MAT_SUB_HEADER);
	if (!bed->matrix) matrix_perror(file_in->fname);
		
	num_projs = bed->matrix->xdim/rebin;
	num_views = bed->matrix->ydim/mash;
	npixels = num_projs*num_views;
	nblks = (npixels*sizeof(float)+511)/512;
	file_pos = 3*MatBLKSIZE + (pl_out-1)*npixels*sizeof(float);
	if (fseek(file_out->fptr, file_pos, 0) == EOF) 
		crash("%s : fseek error\n",file_out->fname);
	for (i=pl_in; i<=pl_in_end; i++) {
		slice = matrix_read_slice(file_in,bed->matrix,i-1,0);
		matrix_float(slice);
		matrix_rebin(slice,rebin,mash,mean);
		fdata = (float*)slice->data_ptr;
		host_data_to_file(fdata,nblks,IeeeFloat);
		if ( (count=fwrite(fdata,sizeof(float),npixels,file_out->fptr)) !=
			npixels) crash("%s : fwrite error\n",file_out->fname);
		free_matrix_data(slice);
	}
	if (verbose) printf("copy %s,1,[%d-%d],1,0,%d	===> %s,1,[%d-%d],1 done\n",
		file_in->fname, pl_in, pl_in_end, bed->bed, file_out->fname,
		pl_out, pl_out+pl_in_end - pl_in);
}

#ifdef __osf__
wb_scan_merge(int pl_out, MatrixData *slice1, float w1,
BedPosition *bed_2, int pl2, float w2, int mash, int rebin)
#else
wb_scan_merge(pl_out, slice1,w1,bed_2,pl2,w2,mash, rebin)
BedPosition *bed_2;
MatrixData *slice1;
int pl_out, pl2,mash, rebin;
float w1, w2;
#endif
{
	int i, count;
	int file_pos, nblks;
	struct MatDir *matdir_2;
	float  *fdata1, *fdata2, *fdata;
	int num_projs, num_views, npixels;
	BedPosition *bed_1;
	int pl1;
	int mean=0;		/* add statistics when rebinning */
	MatrixData  *slice2;

	matdir_2 = &bed_2->matdir;
	pl1 = 42 + pl2;
	bed_1 = bed_2 - 1;
	if (!bed_2->matrix) {
		bed_2->matrix = matrix_read(file_in, matdir_2->matnum, MAT_SUB_HEADER);
		if (!bed_2->matrix) matrix_perror(file_in->fname);
	}

	num_projs = slice1->xdim/rebin;
	num_views = slice1->ydim/mash;
	npixels = num_projs*num_views;
	nblks = (npixels*sizeof(float)+511)/512;
	fdata = (float*)calloc(nblks,512);
	file_pos = 3*MatBLKSIZE + (pl_out-1)*npixels*sizeof(float);
	if (fseek(file_out->fptr, file_pos, 0) == EOF) 
		crash("%s : fseek error\n",file_out->fname);
	if (slice1->data_type != IeeeFloat) matrix_float(slice1);
	slice2 = matrix_read_slice(file_in,bed_2->matrix,pl2-1,0);
	if (slice2->data_type != IeeeFloat) matrix_float(slice2);
	matrix_rebin(slice1,rebin,mash,mean);
	matrix_rebin(slice2,rebin,mash,mean);
	fdata1 = (float*)slice1->data_ptr;
	fdata2 = (float*)slice2->data_ptr;
	for (i=0; i<npixels; i++)  {
		fdata[i] = w1*fdata1[i]+w2*fdata1[i];
	}
	host_data_to_file(fdata,nblks,IeeeFloat);
	if (fwrite(fdata, sizeof(float),npixels,file_out->fptr) != npixels) 
		crash("%s : fwrite error\n",file_in->fname);

	if (verbose) printf("merge %g * 1,%d,1,0,%d + %g * 1,%d,1,0,%d	===> %s,1,%d,1 done\n",
		w1,pl1,bed_1->bed,w2,pl2,bed_2->bed,file_out->fname,pl_out);
	free(fdata);
	free_matrix_data(slice1);
	free_matrix_data(slice2);

}

int main(argc,argv)
int argc;
char **argv;
{
	BedPosition tmp, *bed_positions = NULL, *bed_1, *bed_2;
	Main_header *mh;
	int i=0, j=0, count=0, sorted=0;
	int plane = 1, mash=1, rebin=1;
	float bed_pos0, *bed_offsets;
	char *all_0;
	char *x_beds;
	int matnum, blkno, nblks, data_size;
	char out_name[128], bed_pos_file[128];
	struct MatDir matdir, dir_entry;
	FILE *fp;
	MatrixData *slice_buf[5];
	int norm_flag = 0;
	extern int optind, opterr;
	extern char *optarg;
	
	in_name[0] = '\0'; out_name[0] = '\0'; norm_name[0] = '\0';
	bed_pos_file[0] = '\0';
	x_beds = calloc(MAX_BEDS,1);
	while ((i = getopt (argc, argv, "i:o:N:x:p:m:r:v")) != EOF) {
	switch(i) {
	case 'v' : verbose = 1;
		break;
	case 'N' :	/* norm file */
			strncpy(norm_name,optarg,127); norm_name[127] = '\0';
			norm_flag = 1;
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
	file_in = matrix_open(in_name, MAT_READ_ONLY, Short3dSinogram);
	if (file_in == NULL){
		matrix_perror(in_name);
		exit(1);
	}
	count =	file_in->mhptr->num_bed_pos+1;
	if (count < 2) 
		fprintf(stderr,"warning: %s is not a multi bed position file\n");
	bed_positions = (BedPosition*)calloc(sizeof(BedPosition),count);
	bed_positions = (BedPosition*)calloc(sizeof(BedPosition),count);
	bed_pos0 = file_in->mhptr->init_bed_position;
	bed_offsets = file_in->mhptr->bed_offset;
	if (bed_pos_file[0] && (fp=fopen(bed_pos_file,"r"))!= NULL) {
		for (i=0;i<count; i++)
			if (fscanf(fp,"%g",bed_offsets+i) != 1) break;
		fclose(fp);
	}
	for (i=0, j=0; i<count; i++) {
		if (x_beds[i]) continue;
		matnum = mat_numcod(1,1,1,0,i);
		if (matrix_find(file_in,matnum,&matdir) < 0 ) {
			fprintf(stderr,"matrix 1,1,1,0,%d not found\n",i);
			exit(1);
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

/* if nomalization required, create temp file name */
	if (norm_flag) {
		printf("Normalization file : %s\n",norm_name);
		tmpnam(tmp_file);
	}

/*	output file overwrite is checked at executive level
	if (access(out_name,F_OK) == 0) {
		fprintf(stderr, "Warning: %s is overwritten\7\n",out_name);
		usage();
	}
*/
	memcpy(&mhead,file_in->mhptr,sizeof(Main_header));
	mhead.num_planes = 42+((5+37)*(count-1))+5;
	mhead.file_type = AttenCor;
	for (i=0;i<mhead.num_bed_pos;i++) mhead.bed_offset[i] = 0.0;
	mhead.num_bed_pos = 0;
	mhead.init_bed_position = bed_positions[0].position;
	file_out = matrix_create(out_name,MAT_CREATE_NEW_FILE, &mhead);
	if (!file_out) matrix_perror(out_name);

	bed_1 = bed_positions;
	bed_1->matrix = matrix_read(file_in,bed_1->matdir.matnum,MAT_SUB_HEADER);
	if (!bed_1->matrix) matrix_perror(file_in->fname);
	scan_sh = (Scan3D_subheader*)bed_1->matrix->shptr;

	memset(&atten_sh,0,sizeof(Attn_subheader));
	atten_sh.data_type = IeeeFloat; 
	atten_sh.storage_order = 1; 			/* force sino mode for easy handling */
        atten_sh.span= scan_sh->axial_compression;
        atten_sh.ring_difference=scan_sh->ring_difference;
	atten_sh.num_z_elements = atten_sh.z_elements[0] = mhead.num_planes;
	atten_sh.num_angles = scan_sh->num_angles/mash;
	atten_sh.y_resolution = scan_sh->v_resolution*mash;
	atten_sh.num_r_elements = scan_sh->num_r_elements/rebin;
	atten_sh.x_resolution = scan_sh->x_resolution*rebin;
	atten_sh.scale_factor = 1.;
	data_size = mhead.num_planes*atten_sh.num_angles*atten_sh.num_r_elements*
		sizeof(float);
	nblks = 2 + (data_size+511)/MatBLKSIZE;
	matnum = mat_numcod(1,1,1,0,0);
	if (matrix_find(file_out, matnum, &matdir) == -1) {
		blkno = mat_enter(file_out->fptr,file_out->mhptr, matnum, nblks) ;
		dir_entry.matnum = matnum ;
		dir_entry.strtblk = blkno ;
		dir_entry.endblk = dir_entry.strtblk + nblks - 1 ;
		dir_entry.matstat = 1 ;
		insert_mdir(dir_entry, file_out->dirlist) ;
		matdir = dir_entry ;
	}
	all_0 = calloc(512,1);
	mat_write_attn_subheader(file_out->fptr,file_out->mhptr, matdir.strtblk,&atten_sh);
	if (fseek(file_out->fptr, (blkno+1)*MatBLKSIZE, 0) == EOF)
		crash("%s : fseek error\n", file_out->fname);
	for (i=2; i<=nblks; i++)
		if (fwrite( all_0, 1, 512, file_out->fptr) != 512)
			crash("%s : fwrite error\n", file_out->fname);
	free(all_0);

	if (norm_flag) apply_norm(0);
	wb_scan_copy(plane,bed_1,1,42,mash,rebin); plane += 42;

	for (i=1; i<count; i++) {
		bed_1 = &bed_positions[i-1]; bed_2 = &bed_positions[i];

/* load the bed_1 last 5 slices in slices buffer */
		for (j=0;j<5;j++) slice_buf[j] =
			matrix_read_slice(file_in,bed_1->matrix,43+j,0);

/* merge bed_1 5 slices buffer with the bed_2 first 5 slices.  */
		if (norm_flag) apply_norm(i);
		wb_scan_merge( plane++, slice_buf[0], .832, bed_2, 1, .179, mash,rebin);
		wb_scan_merge( plane++, slice_buf[1], .698, bed_2 ,2, .379, mash,rebin);
		wb_scan_merge( plane++, slice_buf[2], .533, bed_2, 3, .568, mash,rebin);
		wb_scan_merge( plane++, slice_buf[3], .341, bed_2, 4, .729, mash,rebin);
		wb_scan_merge( plane++, slice_buf[4], .136, bed_2, 5, .864, mash,rebin);

/*
 * copy the bed_2 middle slices
 */
		wb_scan_copy(plane,bed_2,6,42, mash,rebin); plane += 37;
		free_matrix_data(bed_1->matrix);
		bed_1->matrix = 0;
	}

	bed_1 = &bed_positions[count-1];
	wb_scan_copy(plane,bed_1,43,47,mash,rebin); plane += 5;
	free_matrix_data(bed_1->matrix);
	matrix_close(file_in);
	if (norm_flag) unlink(tmp_file);
	matrix_close(file_out);
}
