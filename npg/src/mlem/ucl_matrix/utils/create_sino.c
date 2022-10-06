/* @(#)create_sino.c	1.5 8/9/93 */

#include "matrix.h"
#include <stdlib.h>
#include <strings.h>

static char sccsid[]="@(#)create_sino.c	1.5 8/9/93 -> corrected UCL 2/94";
static char prgnam[]="create_sino";

main( int argc, char **argv)
{
	MatrixFile *mp1;
	MatrixData *mat=NULL;
	Scan3D_subheader *sh;
	int i, matnum, off, off_z, j,  nv;
	short int *p1;


	if (argc<1)
		crash( "%s\nusage: %s infile \n", sccsid, prgnam);
	mp1 = matrix_open( argv[1], MAT_READ_WRITE, Short3dSinogram);
	if (!mp1) matrix_perror( argv[0]);
	matnum = mat_numcod( 1, 1, 1, 0, 0);
	mat = matrix_read( mp1, matnum, GENERIC);
	if (mat) {
printf("mat exist %d %d %d\n",mat->xdim,mat->ydim,mat->zdim);
		p1 = (short int*) mat->data_ptr;
		nv = mat->xdim*mat->ydim;
		memset(p1,0,nv*mat->zdim*sizeof(short));
/*
		off_z = (int)(mat->zdim/2* mat->xdim*mat->ydim + mat->xdim/2);
*/
			off_z = (int)(mat->zdim/2*mat->xdim+ mat->xdim/2);
		for (j = 0; j<mat->ydim; j++){
			off = (int)(off_z+mat->zdim*mat->xdim*j);
			p1[off] = 1000;
		}
		mat->scale_factor = 1;
		mat->data_min = 0;
		mat->data_max = 1000;
		sh = (Scan3D_subheader*) mat->shptr;
		sh->scan_min = 0;
		sh->scan_max = 1000;
		sh->scale_factor = 1.;
		
	  	matrix_write( mp1, matnum, mat);
	}
	printf("complete\n");
	matrix_close( mp1);
	exit(0);
}
