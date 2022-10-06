#include <values.h>
#include <ecat/matrix.h>
#include <stdio.h>
#include "contours.h"

int main (argc, argv)
int  argc;
char** argv;
{
    MatrixFile *mptr1;
    MatrixData *matrix;
    char fname[256];
    int matnum, level;
	int i,sx, sy;
	short* data;
	u_char* res_data;

	if (argc < 2) crash("usage : mat_contours specs\n");
    matspec( argv[1], fname, &matnum);
    mptr1 = matrix_open( fname, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
    if (!mptr1)
      crash( "%s: can't open file '%s'\n", argv[0], fname);
    matrix = matrix_read( mptr1, matnum, UnknownMatDataType);
    if (!matrix) crash( "%s: specified matrix not found\n", argv[0]);
	fprintf(stderr,"type %d \n",matrix->data_type);
	sx = matrix->xdim; sy = matrix->ydim;
	fprintf(stderr,"matrix size %d %d\n",sx,sy);
    fwrite(&sx,sizeof(int),1,stdout);
    fwrite(&sy,sizeof(int),1,stdout);
	level = (int)(matrix->data_max/(matrix->scale_factor*2));
	res_data = greater_contours((short*)matrix->data_ptr,sx,sy,level);
	data = (short*) malloc(sx*sy*sizeof(short));
	for (i=0; i<sx*sy; i++) data[i] = res_data[i];
	fwrite(data,sizeof(short),sx*sy,stdout);
	return 1;
}
