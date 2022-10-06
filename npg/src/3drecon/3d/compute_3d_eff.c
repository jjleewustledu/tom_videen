/*$Id: compute_3d_eff.c,v 1.2 1995/01/30 16:04:48 ty7777 Exp $*/
/*$Log: compute_3d_eff.c,v $
 * Revision 1.2  1995/01/30  16:04:48  ty7777
 * Added program version.
 **/

/* @(#)compute_3d_eff.c	1.10 12/29/92 */

static char sccsid[]="@(#)compute_3d_eff.c	1.10 12/29/92 Copyright 1991-92, CTI Pet Systems, Inc.";
static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/compute_3d_eff.c,v 1.2 1995/01/30 16:04:48 ty7777 Exp $";

#include "matrix.h"

main( argc, argv)
  int argc;
  char **argv;
{

#define MAX_RINGS 48

	MatrixFile *mptr;
	MatrixData *s1;
	Main_header *mh;
	Scan_subheader *sh;
	FILE *fptr;
	struct Matval mv;
	int nprojs, nviews, ndets, ncrystals, vflag, matnum, frame, data_type;
	int i,j,e,v, *dsums, deta, detb, nring, total, mash, m;
	short int *scans[MAX_RINGS], *s;
	float avg, eff, scalef[MAX_RINGS];

	if (argc<3)
	  crash( "%s\nusage: %s 3dscan out.eff [vflag,nrings,mash,data]\n", sccsid, argv[0]);
	mptr = matrix_open( argv[1], MAT_READ_ONLY, SCAN_DATA);
	if (!mptr) matrix_perror( argv[0]);
	vflag = 0;
	nring = 16;
	mash = 1;
	data_type = 0;
	frame = 1;
	if (argc>3) sscanf( argv[3], "%d,%d,%d,%d", &vflag, &nring, &mash, &data_type);
	matnum = matnumfx( 0, 0, vflag, frame, data_type);
	if (vflag < 0) matnum = mat_numcod( frame, 1, 1, data_type, 0);
	s1 = matrix_read( mptr, matnum, MAT_WORD);
	sh = (Scan_subheader*) s1->shptr;
	nprojs = sh->dimension_1;
	nviews = sh->dimension_2;
	ndets = nviews*2*mash;
	matrix_free( s1);
	ncrystals = ndets*nring;
	dsums = (int *)calloc( ncrystals, sizeof(int));
	for (i=0; i<nring; i++)
	{
	  matnum = matnumfx( i, i, vflag, frame, data_type);
	  if (vflag < 0) matnum = mat_numcod( frame, 2*i+1, 1, data_type, 0);
	  mat_numdoc( matnum, &mv);
	  s1 = matrix_read( mptr, matnum, MAT_WORD);
	  if (!s1) matrix_perror( argv[0]);
	  scans[i] = (short int*) s1->data_ptr;
	  scalef[i] = s1->scale_factor;
	  free( s1->shptr);
	  free( s1 );
	}
	matrix_close( mptr);
	for (v=0; v<nviews; v++)
	  for (e=0; e<nprojs; e++)
	  {
	     for (m=0; m<mash; m++)
	     {
		ve_to_dets( v*mash+m, e-nprojs/2, &deta, &detb, ndets);
		for (j=0; j<nring; j++)
		{
		  s = scans[j];
		  dsums[deta+ndets*j] += s[e+v*nprojs]*scalef[j];
		  dsums[detb+ndets*j] += s[e+v*nprojs]*scalef[j];
		}
	      }
	  }
	avg = 0.0;
	for (i=0; i<ncrystals; i++)
	  avg += (float)dsums[i];
	avg = avg/(float)ncrystals;
	fptr = fopen( argv[2], "w");
	for (i=0; i<ncrystals; i++)
	{
	  eff = avg/(float)dsums[i];
	  fprintf( fptr, "%10d %3d %e\n", dsums[i], 1, eff);
	}
	close( fptr);
}
