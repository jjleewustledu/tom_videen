#include "matrix.h"
#include <string.h>
#include <stdio.h>
#define DIVIDE 0
#define MULT 1

char *opstr[] = { "divided", "multiplied" };

static usage() {
	fprintf(stderr,
	"usage: merge_attn file1 file2 [-divide | -mult] [-verbose]\n");
	fprintf(stderr, "warning : file1 is overwritten as the result\n");
	exit(1);
}


main(argc,argv)
int argc;
char **argv;
{
	int c=0, verbose=0, op=MULT;
	char *file1=NULL, *file2=NULL;
	MatrixFile *mptr1, *mptr2;
	MatDirNode *node;
	Attn_subheader *ah=NULL;
	float *view_data1=NULL, *view_data2=NULL, *tmp_data=NULL;
	int i, view, view_size, file_pos;
	int num_projs, num_planes, num_views;

	while (--argc) {
		argv++;
		if (strncmp(*argv,"-d",2) == 0) op = DIVIDE;
			else if (strncmp(*argv,"-m",2) == 0) op = MULT;
				else if (strncmp(*argv,"-v",2) == 0) verbose = 1;
					else if (!file1) file1 = strdup(*argv);
						else if (!file2) file2 = strdup(*argv);
							else usage();
	}
	if (!file1 || !file2) usage();
	if (verbose) 
		fprintf(stderr,"%s is %s by %s\n", file1, opstr[op], file2);
	if ((mptr1=matrix_open(file1, MAT_OPEN_EXISTING, AttenCor)) == NULL)
		crash("can't open Attenuation file %s\n",file1);
	if ((mptr2=matrix_open(file2, MAT_READ_ONLY, AttenCor)) == NULL)
		crash("can't open Attenuation file %s\n",file2);
	node = mptr1->dirlist->first;
	while (node) {
		if (!ah) ah = (Attn_subheader*) calloc( 1, sizeof(Attn_subheader));
		mat_read_attn_subheader(mptr1->fptr, mptr1->mhptr, node->strtblk,ah);
		num_projs = ah->num_r_elements;
		num_planes = ah->num_z_elements;
		num_views = ah->num_angles;
		view_size = num_projs*num_planes;
		if (!view_data1) {
			view_data1 = (float*)calloc(sizeof(float),view_size);
			view_data2 = (float*)calloc(sizeof(float),view_size);
			if (ntohs(1) != 1)
				tmp_data = (float*)calloc(sizeof(float),view_size);
		}
		file_pos = node->strtblk*MatBLKSIZE;
		for (view=0; view <num_views; view++) {	/* for each view */
										/* skip to view position */
			if (fseek( mptr1->fptr, file_pos, 0) == -1)
				crash("%s : fseek error\n", mptr1->fname);
			if (fseek( mptr2->fptr, file_pos, 0) == -1)
				crash("%s : fseek error\n", mptr2->fname);
										/* read data */
			if (fread(view_data1,sizeof(float),view_size,mptr1->fptr) !=
				view_size) crash("%s : fread error\n", mptr1->fname);
			if (fread(view_data2,sizeof(float),view_size,mptr2->fptr) !=
				view_size) crash("%s : fread error\n", mptr2->fname);
			if (ntohs(1) != 1) {
				swab(view_data1,tmp_data,view_size*sizeof(float));
				swaw(tmp_data,view_data1,view_size*2);
				swab(view_data2,tmp_data,view_size*sizeof(float));
				swaw(tmp_data,view_data2,view_size*2);
			}
										/* divide or mult folowing op code */
			if (op == DIVIDE)
				for (i=0; i<view_size; i++) view_data1[i] /= view_data2[i];
			else for (i=0; i<view_size; i++) view_data1[i] *= view_data2[i];
										/* skip to view position for writing */
			if (fseek( mptr1->fptr, file_pos, 0) == -1)
				crash("%s : fseek error for write\n", mptr1->fname);
										/* write result to file1 */
			if (ntohs(1) != 1) {
				swab(view_data1,tmp_data,view_size*sizeof(float));
				swaw(tmp_data,view_data1,view_size*2);
			}
			if (fwrite(view_data1,sizeof(float),view_size,mptr1->fptr) !=
				view_size) crash("%s : fread error\n", mptr1->fname);
										/* set position to next view */
			file_pos += view_size*sizeof(float);
		}
										/* next matrix */
		node = node->next;
	}
	matrix_close(mptr1);
	matrix_close(mptr2);
}
