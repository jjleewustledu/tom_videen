/* @(#)scanmath.c	1.1 4/26/91 */

#ifndef	lint
static char sccsid[]="@(#)scanmath.c	1.1 4/26/91 Copyright 1990 CTI Pet Systems, Inc.";
#endif	lint

/*
 * Feb-1996 :Updated by Sibomana@topo.ucl.ac.be for ECAT 7.0 support
 */
   

#include "matrix.h"
#include <math.h>


main( argc, argv)
  int argc;
  char **argv;
{
	MatrixFile *mptr=NULL;
	MatDirNode *node;
	MatrixData *scan=NULL;
	Scan3D_subheader *sh=NULL;
	short *data;
	int t=0, scan_len=0;
	float activity=0;
	int i, npixels;

	if (argc<2) {
		fprintf(stderr,"usage : scanTAC dynamic_scan_file\n");
		exit(1);
	}
	mptr = matrix_open( argv[1], MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
	if (!mptr)
	  crash( "%s: can't open file '%s'\n", argv[0], argv[1]);
	if (mptr->mhptr->file_type != Short3dSinogram)
			crash("%s is not a Sinogram\n", argv[1]);
	
	node = mptr->dirlist->first;
	printf("#xy\n");
	while (node) {
		scan = matrix_read(mptr,node->matnum,GENERIC);
		data = (short*)scan->data_ptr;
		sh = (Scan3D_subheader*)scan->shptr;
		npixels = scan->xdim*scan->ydim*scan->zdim;
		activity = 0;
		for (i=0; i<npixels; i++) activity += data[i];
		scan_len = sh->frame_duration/1000;	/* millisec to sec */
		activity /= scan_len;
		printf("%d\t%g\n",t,activity);
		free_matrix_data(scan);
		node = node->next;
		t += scan_len;
	}
	matrix_close(mptr);
}
