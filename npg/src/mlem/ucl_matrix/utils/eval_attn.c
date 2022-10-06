#include "matrix.h"
#include <stdio.h>

main(argc,argv)
int argc;
char **argv;
{
	MatrixFile *mptr;
	MatrixData *volume, *slice;
	MatDirNode *node;
	float f, *fdata, sum, sumsq, stdv, ave, lo, hi;
	int i, j, npixels, segment=0;

	if (argc < 2) exit(0);
	if ((mptr=matrix_open(argv[1], MAT_READ_ONLY, AttenCor)) == NULL)
		crash("can't open Attenuation file %s\n",argv[1]);
	node = mptr->dirlist->first;
	while (node) {
		volume = matrix_read(mptr,node->matnum,MAT_SUB_HEADER);
		for (i=0; i<volume->zdim; i++) {
			slice = matrix_read_slice(mptr,volume,i,segment);
			fdata = (float*)slice->data_ptr;
			npixels = slice->xdim*slice->ydim;
			lo = hi = sum = fdata[0];
			sumsq = sum*sum;
			for (j=1; j<npixels; j++) {
				f = fdata[j];
				sum += f;
				sumsq += f*f;
				if (hi < f) hi = f;
				else if (lo>f) lo = f;
			}
			ave = sum/npixels;
			stdv = sqrt((sumsq-(sum*sum)/npixels)/(npixels-1));
			printf("slice %d : mean = %g, stdv = %g, min = %g, max = %g\n",
				i+1, ave, stdv, lo,hi);
			free_matrix_data(slice);
		}
		free_matrix_data(volume);
		node = node->next;
	}
	matrix_close(mptr);
}
