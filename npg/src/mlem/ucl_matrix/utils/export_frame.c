#include <stdio.h>
#include <ecat/matrix.h>
main( argc, argv)
  int argc;
  char **argv;
{
    int matnum;
    char fname[256];
    MatrixFile *matrix_file;
	FILE *out;
    MatrixData *matrix;
	int data_size;

    if (argc<3) crash( "usage    : %s frame_specs out_file\n", argv[0]);
    matspec( argv[1], fname, &matnum);
	matrix_file = matrix_open( fname, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
	if (matrix_file == NULL) crash("%s : no such file\n",fname);
	out = fopen(argv[2],"w");
	if (out == NULL) crash("%s : can't create\n",argv[2]);
	matrix = matrix_read(matrix_file, matnum, UnknownMatDataType);
	if (matrix == NULL) crash("%s : no such frame\n",argv[1]);
	fwrite(&matrix->xdim,sizeof(int),1,out);
	fwrite(&matrix->ydim,sizeof(int),1,out);
	fwrite(&matrix->zdim,sizeof(int),1,out);
	fwrite(&matrix->scale_factor,sizeof(float),1,out);
	data_size = matrix->xdim*matrix->ydim*matrix->zdim;
	if (fwrite(matrix->data_ptr,sizeof(short),data_size,out) != data_size)
		perror(argv[2]);
	matrix_close(matrix_file);
	fclose(out);
}
