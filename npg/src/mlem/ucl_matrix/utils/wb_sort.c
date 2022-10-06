#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct _BedPosition {
	int bed;
	float position;
} BedPosition;

static usage() {
	fprintf(stderr,"usage : wb_sort multi_bed_file bed_pos_file\n");
	fprintf(stderr,"The program does nothing if the output file exists\n");
	exit(1);
}

int main(argc,argv)
int argc;
char **argv;
{
	BedPosition tmp, *bed_positions = NULL;
	MatrixFile *mptr;
	FILE *fp;
	float bed_pos0, *bed_offsets;
	int i=0, count, sorted = 0;
	
	if (argc < 2) usage();
	mptr = matrix_open(argv[1], MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
	if (mptr == NULL) {
		fprintf(stderr,"can't open %s matrix file\n",argv[i]);
		exit(1);
	}
	count = mptr->mhptr->num_bed_pos+1;
	if (count < 3) {
		fprintf(stderr,"%s is not a multi bed position file\n");
		exit(1);
	}
	bed_positions = (BedPosition*)calloc(sizeof(BedPosition),count);
	bed_pos0 = mptr->mhptr->init_bed_position;
	bed_offsets = mptr->mhptr->bed_offset;
	bed_positions[0].bed = 0;
	bed_positions[0].position = bed_pos0;
/* bed offset position 1 is not recorded, use position 2 */
	bed_positions[1].bed = 1;
	bed_positions[1].position = bed_pos0 + (bed_offsets[0]/2);
	if (argc > 2 && (fp=fopen(argv[2],"r"))!= NULL) {
		for (i=0;i<count; i++) 
			if (fscanf(fp,"%g",bed_offsets+i) != 1) break;
		fclose(fp);
	}
	for (i=2; i<count; i++) {
		bed_positions[i].bed = i;
		bed_positions[i].position = bed_pos0 + bed_offsets[i-2];
	}
	matrix_close(mptr);
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
	for (i=0; i<count; i++) 
		printf("%d : %g\n",bed_positions[i].bed,
			bed_positions[i].position);
	for (i=0; i<count; i++) {
		if (i>0) printf("merge %d[43-47] with %d[1-5]\n",
			bed_positions[i-1].bed,bed_positions[i].bed);
		else printf("copy %d[1-5]\n",bed_positions[i].bed);
		printf("copy %d[6-42]\n",bed_positions[i].bed);
		if (i == count-1)
			printf("copy %d[43-47]\n",bed_positions[i].bed);
	}
}
