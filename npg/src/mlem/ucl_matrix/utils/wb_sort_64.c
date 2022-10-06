#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct _FilePosition {
	char *filename;
	float position;
} FilePosition;

static usage() {
	fprintf(stderr,"usage : wb_build file1 ... filen wb_file\n");
	fprintf(stderr,"eg : wb_build p01572fd*.img p01572fdg.img to assemble\n");
	fprintf(stderr,"p01572fd1.img ... p01572fd11.img into p01572fdg.img\n");
	fprintf(stderr,"The program does nothing if the output file exists\n");
	exit(1);
}

int main(argc,argv)
int argc;
char **argv;
{
	FilePosition tmp, *file_positions = NULL;
	MatrixFile * mptr;
	int i=0, count, sorted = 0;
	
	if (argc < 3) usage();
	count = argc-2;
	file_positions = (FilePosition*)calloc(sizeof(FilePosition),count);
	mptr = matrix_open(mptr, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
	for (i=1; i<=count; i++) {
		mptr = matrix_open(argv[i], MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
		if (mptr == NULL) {
			fprintf(stderr,"can't open %s matrix file\n",argv[i]);
			exit(1);
		}
		file_positions[i-1].filename = argv[i];
		file_positions[i-1].position = mptr->mhptr->init_bed_position;
		matrix_close(mptr);
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
	for (i=0; i<count; i++) {
		if (i>0) printf("merge %s[43-47] with %s[1-5]\n",
			file_positions[i-1].filename,file_positions[i].filename);
		else printf("copy %s[1-5]\n",file_positions[i].filename);
		printf("copy %s[6-42]\n",file_positions[i].filename);
		if (i == count-1)
			printf("copy %s[43-47]\n",file_positions[i].filename);
	}
}
