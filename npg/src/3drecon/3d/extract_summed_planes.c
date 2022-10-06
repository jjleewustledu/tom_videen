/*$Id: extract_summed_planes.c,v 1.2 1995/01/30 16:16:53 ty7777 Exp $*/
/*$Log: extract_summed_planes.c,v $
 * Revision 1.2  1995/01/30  16:16:53  ty7777
 * Added program version.
 **/

/* @(#)extract_summed_planes.c	1.13 10/27/92 */

#include "matrix.h"

static char sccsid [] = "@(#)extract_summed_planes.c	1.13 10/27/92 Copyright 1992 CTI Pet Systems, Inc.";
static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/extract_summed_planes.c,v 1.2 1995/01/30 16:16:53 ty7777 Exp $";

static char *prgnam;

#define max(a,b) ((a)>(b)?(a):(b))

main( argc, argv)
  int argc;
  char **argv;
{
	MatrixFile *mptr1, *mptr2;
	MatrixData *scan=NULL;
	Scan_subheader *sh;
	Main_header *mhptr;
	short int *idata;
	float *fdata=NULL, *dptr;
	int vflag, nrings, frame, maxrd;
	int matnum, np, znum, a, b, numz;
	int dflag, nvals=0;
	int i, num_planes, oframe, odata;
	int c;
	char *scan_3d, *scan_2d;
	extern char *optarg;
	extern int optind, opterr;

	prgnam = argv[0];
	if (argc<3)
	{
	  crash( "%s\nusage: %s [-o vflag] [-r num_rings] [-f frame] [-m maxrd] [-x oframe] scan_3d scan_2d\n", sccsid, prgnam);
	}
	vflag = 0;
	nrings = 16;
	frame = 1;
	dflag = 0;
	maxrd = -1;
	oframe = 1;
	odata = 0;
	while ( (c=getopt( argc, argv, "o:r:f:m:x:")) != -1)
	  switch (c)
	  {
		case 'o':	/* -o : use old plane numbering */
			vflag = atoi(optarg);
			break;
		case 'r':	/* -r val : specify number of crystal rings [default 16] */
			nrings = atoi(optarg);
			break;
		case 'f':	/* -f : specify frame to extract */
			sscanf( optarg, "%d,%d", &frame, &dflag);
			break;
		case 'm':	/* -m : specify maximum ring difference */
			sscanf( optarg, "%d", &maxrd);
			break;
		case 'x':	/* -x : specify output frame */
			sscanf( optarg, "%d,%d", &oframe, &odata);
	  }
	scan_3d = argv[optind++];
	scan_2d = argv[optind++];
	mptr1 = matrix_open( scan_3d, MAT_READ_ONLY, SCAN_DATA);
	if (!mptr1) matrix_perror( prgnam);
	mhptr = matrix_init_main_header( argv[2], SCAN_DATA, mptr1->mhptr);
	mptr2 = matrix_create( scan_2d,
		(oframe == 1 && odata == 0) ? MAT_CREATE : MAT_OPEN_EXISTING,
		mhptr);
	if (!mptr2) matrix_perror( prgnam);
	if (maxrd == -1) maxrd = nrings;
	numz = 2*nrings-1;
	for (znum=0; znum<numz; znum++)
	{
	  np = 0;
	  bzero( fdata, nvals*sizeof(float));
	  for (a=0; a<nrings; a++)
	  {
	    b = znum - a;
	    if ((b<0)||(b>nrings-1)||(a-b>maxrd-1)||(b-a>maxrd-1)) continue;
	    matnum = matnumfx( a, b, vflag, frame, dflag);
	    if (scan) matrix_free( scan);
	    scan = matrix_read( mptr1, matnum, MAT_WORD);
	    if (!scan)
	      crash( "%s: can't read matrix for %d,%d (%d)\n", argv[0], a, b, matnum);
	    idata = (short int*) scan->data_ptr;
	    nvals = scan->xdim*scan->ydim;
	    if (!fdata) fdata = (float*) calloc( nvals, sizeof(float));
	    for (i=0; i<nvals; i++) fdata[i] += scan->scale_factor*idata[i];
	    np++;
	  }
	  convert_float_scan( scan, fdata);
	  scan->scale_factor = scan->scale_factor / (float) np;
	  sh = (Scan_subheader*) scan->shptr;
	  sh->scale_factor = sh->scale_factor / (float) np;
	  matnum = mat_numcod( oframe, znum+1, 1, odata, 0);
	  matrix_write( mptr2, matnum, scan);
	}
	matrix_close( mptr1);
	matrix_close( mptr2);
	exit(0);
}
