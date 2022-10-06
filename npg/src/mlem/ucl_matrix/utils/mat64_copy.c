/* @(#)matcopy.c	1.4 7/10/92 */

#include "matrix.h"

#ifndef lint
static char sccsid[]="@(#)matcopy.c	1.4 7/10/92 Copyright 1990 CTI Pet Systems, Inc.";
#endif lint

main( argc, argv)
  int argc;
  char **argv;
{
	MatrixFile *mptr1, *mptr2;
	MatrixData *matrix;
	Main_header proto;
	char fname[256];
	int matnum1=0, matnum2=0;

	matspec( argv[1], fname, &matnum1);
	mptr1 = matrix_open( fname, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
	if (!mptr1)
	  crash( "%s: can't open file '%s'\n", argv[0], fname);
	matrix = matrix_read( mptr1, matnum1, GENERIC);
	if (!matrix)
	  crash( "%s: specified matrix not found\n", argv[0]);

	matspec( argv[2], fname, &matnum2);
	if (matnum2 == 0) matnum2 = matnum1; 	/* use same specifications */
	proto = *mptr1->mhptr;
	proto.sw_version = 6;
	mptr2 = matrix_create( fname, MAT_OPEN_EXISTING, &proto);
	if (!mptr2)
	  matrix_perror( argv[0]);
	matrix_write( mptr2, matnum2, matrix);
	matrix_close( mptr1);
	matrix_close( mptr2);
}
