#include <values.h>
#include "matrix.h"
#include <stdio.h>

int main (argc, argv)
int  argc;
char** argv;
{
    MatrixFile *mptr;
    MatrixData *matrix;
    char fname[256];
    int matnum;
	short type;
	caddr_t data;
	extern MatrixData* scan3d_read(), *attn_read();

	if (argc < 2) crash("usage : read_cti specs\n");
    matspec( argv[1], fname, &matnum);
    mptr = matrix_open( fname, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
    if (!mptr)
      crash( "%s: can't open file '%s'\n", argv[0], fname);
    matrix = matrix_read( mptr, matnum, UnknownMatDataType);
    if (!matrix) crash( "%s: specified matrix not found\n", argv[0]);
	fprintf(stderr,"type %d \n",matrix->data_type);
	switch(matrix->data_type) {
	case ByteData:
		type = 0;
		break;
	default:
	case VAX_Ix2:
	case SunShort:
		type = 1;
		break;
	case SunLong :
		type = 2;
		break;
	case VAX_Rx4:
	case IeeeFloat:
		type = 3;
		break;
	}
	fprintf(stderr,"matrix size %d %d %d\n",matrix->xdim,matrix->ydim,
		matrix->zdim);
    fwrite(&type,sizeof(short),1,stdout);
    fwrite(&matrix->xdim,sizeof(short),1,stdout);
    fwrite(&matrix->ydim,sizeof(short),1,stdout);
    fwrite(&matrix->zdim,sizeof(short),1,stdout);
	fwrite(matrix->data_ptr,1,matrix->data_size,stdout);
	return 1;
}
