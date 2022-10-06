/*$Id: fit_scans.c,v 1.2 1995/01/30 16:18:19 ty7777 Exp $*/
/*$Log: fit_scans.c,v $
 * Revision 1.2  1995/01/30  16:18:19  ty7777
 * Added program version.
 **/

/* @(#)fit_scans.c	1.4 6/6/91 */

#include "matrix.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/fit_scans.c,v 1.2 1995/01/30 16:18:19 ty7777 Exp $";

main( argc, argv)
  int argc;
  char **argv;
{
	MatrixFile *f1, *f2;
	MatrixData *m1, *m2;
	Scan_subheader *sh1, *sh2;
	short int *p1, *p2;
	int i, nvals, matnum, nsinos, rd, znum, nv, a, b;
	float phi, zoff, s1, s2;
	char flag[256];
	double sumx, sumx2, sumy, sumxy, afit, bfit, denom, chisq, d;
	double sumafit, sumbfit, sumchisq;

	if (argc<3)
	  crash( "usage: %s fproj_scan meas_scan\n", argv[0]);
	f1 = matrix_open( argv[1], MAT_READ_ONLY, SCAN_DATA);
	if (!f1) matrix_perror( argv[0]);
	f2 = matrix_open( argv[2], MAT_READ_ONLY, SCAN_DATA);
	if (!f2) matrix_perror( argv[0]);
	sumafit = sumbfit = sumchisq = 0.0;
	nsinos = 0;
	while ((nv=scanf( "%d,%d,%f,%f,%d,%s %d,%d", &rd, &znum, &phi, &zoff, &matnum, flag, &a, &b))==8)
	{
	  nsinos++;
	  m1 = matrix_read( f1, matnum, MAT_WORD);
	  if (!m1) matrix_perror( argv[0]);
	  m2 = matrix_read( f2, matnum, MAT_WORD);
	  if (!m2) matrix_perror( argv[0]);
	  if ((m1->xdim != m2->xdim) || (m1->ydim != m2->ydim))
	    crash( "%s: matrices are not the same size!\n", argv[0]);
	  nvals = m1->xdim*m1->ydim;
	  sumx = sumx2 = sumy = sumxy = 0.0;
	  p1 = (short int*) m1->data_ptr;
	  p2 = (short int*) m2->data_ptr;
	  s1 = m1->scale_factor;
	  s2 = m2->scale_factor;
	  for (i=0; i<nvals; i++)
	  {
	    sumx += s1*p1[i];
	    sumx2 += s1*s1*p1[i]*p1[i];
	    sumy += s2*p2[i];
	    sumxy += s1*p1[i]*s2*p2[i];
	  }
	  denom = (float)nvals*sumx2-sumx*sumx;
	  afit = ((float)nvals*sumxy-sumx*sumy)/denom;
	  bfit = (sumx2*sumy-sumxy*sumx)/denom;
	  chisq = 0.0;
	  for (i=0; i<nvals; i++)
	  {
	    d = (s2*p2[i]-afit*s1*p1[i]-bfit);
	    chisq += d*d;
	  }
	  printf("%2d %12.6e,%12.6e,%12.6e\n", matnum, afit, bfit, chisq/nvals);
	  free_matrix(m1);
	  free_matrix(m2);
	  sumafit += afit;
	  sumbfit += bfit;
	  sumchisq += chisq/nvals;
	}
	printf("Average fit to meas = a*synt+b\n");
	printf("AAVG = %12.6e, BAVG = %12.6e\n",
	  sumafit/nsinos, sumbfit/nsinos);
	printf("Average chisqr = %12.6e\n", sumchisq/nsinos);
}

free_matrix( m)
  MatrixData *m;
{
	free( m->shptr);
	free( m->data_ptr);
	free(m);
}
