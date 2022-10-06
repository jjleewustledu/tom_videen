/*  $Id$
    $Log$ */
/* ===============================================================================
 *  Module:         alignbyfids.c
 *  Date:           20-Jul-95
 *  Author:         Tom Videen
 *  Description:
 *		Find matrix which aligns 2 images usign a set of fiducial points.
 * ===============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nrecipes/kr/nr.h>
#include <nrecipes/kr/nrutil.h>

#define NVAL 2					 /* number of values in input file per line */
#define NDIM 3					 /* number of dimensions */
#define NVAR 6					 /* number of simplex dimensions */
#define NSIM 7					 /* number of simplex vectors */
#define FTOL 1.0e-6

static char     rcsid[] = "$Header$";

char            Usage[] = "Usage: alignbyfids fixed.fid reslice.fid fixed.hdr reslice.hdr\n";
float           matdist ();
float         **fixed;
float         **reslice;
int             nfid = 5;			 /* number of fiducial points */

main (argc, argv)
	int             argc;
	char          **argv;
{
	char           *fidfile1, *fidfile2;
	char           *hdrfile1, *hdrfile2;
	float           dist;
	float          *x, *y, **p;
	int             nfunc;
	int             i, j, k;
	FILE           *fp1, *fp2, *fp3, *fp4;

	static char     rcsid[] = "$Header$";

/* Open input files */

	if (argc != 5) {
		fprintf (stderr, Usage);
		exit (1);
	}
	fidfile1 = argv[1];
	fidfile2 = argv[2];
	hdrfile1 = argv[3];
	hdrfile2 = argv[4];

	fp1 = fopen (fidfile1, "r");
	if (fp1 == (FILE *) NULL) {
		fprintf (stderr, "Error: cannot open %s\n", fidfile1);
		exit (1);
	}
	fp2 = fopen (fidfile2, "r");
	if (fp2 == (FILE *) NULL) {
		fprintf (stderr, "Error: cannot open %s\n", fidfile2);
		exit (1);
	}
	fp3 = fopen (hdrfile1, "r");
	if (fp3 == (FILE *) NULL) {
		fprintf (stderr, "Error: cannot open %s\n", hdrfile1);
		exit (1);
	}
	fp4 = fopen (hdrfile2, "r");
	if (fp4 == (FILE *) NULL) {
		fprintf (stderr, "Error: cannot open %s\n", hdrfile2);
		exit (1);
	}
/*
 *	Read the points
 *	The file contains NVAL sets of values: pixel and mm
 *	The pixel coordinates are read and then replace when the mm coordinates are read.
 */

	fixed = matrix (1, nfid, 1, NDIM + 1);
	reslice = matrix (1, nfid, 1, NDIM + 1);

	for (i = 1; i <= nfid; i++) {
		for (j = 1; j <= NVAL; j++) {
			for (k = 1; k <= NDIM; k++)
				fscanf (fp1, "%f", &fixed[i][k]);
			fixed[i][NDIM + 1] = 1.0;
		}
	}

	for (i = 1; i <= nfid; i++) {
		for (j = 1; j <= NVAL; j++) {
			for (k = 1; k <= NDIM; k++)
				fscanf (fp2, "%f", &reslice[i][k]);
			reslice[i][NDIM + 1] = 1.0;
		}
	}

	fclose (fp1);
	fclose (fp2);
	fclose (fp3);
	fclose (fp4);

/*	Initialize simplex */

	x = vector (1, NVAR);
	y = vector (1, NSIM);
	p = matrix (1, NSIM, 1, NVAR);
	for (i = 1; i <= NSIM; i++) {
		for (j = 1; j <= NVAR; j++)
			x[j] = p[i][j] = (i == (j + 1) ? 30.0 : 0.0);
		y[i] = matdist (x);
	}

	printf ("initial values at the vertices:\n\n");
	for (i = 1; i <= NSIM; i++) {
		printf ("%3d ", i);
		for (j = 1; j <= NVAR; j++)
			printf ("%12.6f ", p[i][j]);
		printf ("%12.6f\n", y[i]);
	}

	simplex (p, y, NVAR, FTOL, matdist, &nfunc);

	printf ("Number of function evaluations: %3d\n", nfunc);
	printf ("function values at the vertices:\n\n");
	for (i = 1; i <= NSIM; i++) {
		printf ("%3d ", i);
		for (j = 1; j <= NVAR; j++)
			printf ("%12.6f ", p[i][j]);
		printf ("%12.6f\n", y[i]);
	}

	x[1] = p[1][1];
	for (i = 2; i <= NSIM; i++) {
		x[i] = p[i][i] = p[i][i] + 15.0;
		y[i] = matdist (x);
	}

	simplex (p, y, NVAR, FTOL, matdist, &nfunc);

	printf ("Number of function evaluations: %3d\n", nfunc);
	printf ("function values at the vertices:\n\n");
	for (i = 1; i <= NSIM; i++) {
		printf ("%3d ", i);
		for (j = 1; j <= NVAR; j++)
			printf ("%12.6f ", p[i][j]);
		printf ("%12.6f\n", y[i]);
	}

	free_matrix (fixed, 1, nfid, 1, NDIM);
	free_matrix (reslice, 1, nfid, 1, NDIM);
	free_matrix (p, 1, NSIM, 1, NVAR);
	free_vector (x, 1, NVAR);
	free_vector (y, 1, NSIM);
}
