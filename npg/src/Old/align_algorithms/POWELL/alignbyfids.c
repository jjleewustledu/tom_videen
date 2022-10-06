/*  $Id$
    $Log$ */
/* ===============================================================================
 *  Module:         alignbyfids.c
 *  Date:           21-Jul-95
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
#include <petutil/matrixlib.h>

#define NVAL 1					 /* number of values in input file per line */
#define NDIM 3					 /* number of dimensions */
#define MSIZE 4					 /* NDIM + 1 */
#define NPRM 6					 /* number of parameters */
#define FTOL 1.0e-7

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
	float         **mat, *aligned;
	float         **x, *p, fret;
	int             iter;
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
 *	Read the points:
 *	The file contains NVAL sets of values: pixel and mm
 *	The pixel coordinates are read and then replace when the mm coordinates are read.
 */

	fixed = matrix (1, nfid, 1, MSIZE);
	reslice = matrix (1, nfid, 1, MSIZE);

	for (i = 1; i <= nfid; i++) {
		for (j = 1; j <= NVAL; j++) {
			for (k = 1; k <= NDIM; k++)
				fscanf (fp1, "%f", &fixed[i][k]);
			fixed[i][MSIZE] = 1.0;
		}
	}

	for (i = 1; i <= nfid; i++) {
		for (j = 1; j <= NVAL; j++) {
			for (k = 1; k <= NDIM; k++)
				fscanf (fp2, "%f", &reslice[i][k]);
			reslice[i][MSIZE] = 1.0;
		}
	}

/*
    printf ("%10.4f  %10.4f  %10.4f\n", fixed[1][1], fixed[1][2], fixed[1][3]);
    printf ("%10.4f  %10.4f  %10.4f\n", reslice[1][1], reslice[1][2], reslice[1][3]);
 */

	fclose (fp1);
	fclose (fp2);
	fclose (fp3);
	fclose (fp4);

/*	Initialize */

	x = matrix (1, NPRM, 1, NPRM);
	p = vector (1, NPRM);
	for (i = 1; i <= NPRM; i++) {
		p[i] = 0;
		for (j = 1; j <= NPRM; j++)
			x[i][j] = ((i == j) ? 1.0 : 0.0);
	}

	dist = matdist (p);
	printf ("Initial error = %12.6f\n", dist);

	powell (p, x, NPRM, FTOL, &iter, &fret, matdist);

	dist = matdist (p);
	printf ("Final error   = %12.6f\n", dist);
	printf ("Number of Iterations: %6d\n\n", iter);
	printf ("Minimum at: \n");
/*
	for (i = 1; i <= NPRM; i++)
		printf ("%12.6f", p[i]);
	printf ("\nMinimum = %12.6f \n\n", fret);
 */
	printf ("   Translation (mm) = ");
	for (i = 1; i <= 3; i++)
		printf ("%10.4f", p[i]);
	printf ("\n   Rotation (deg)   = ");
	for (i = 4; i <= NPRM; i++)
		printf ("%10.4f", p[i]);

	mat = matrix (1, MSIZE, 1, MSIZE);
	aligned = vector (1, MSIZE);
	matcreate (mat, p);

	printf ("\n\nTransformation matrix:\n");
	matrix_print (mat, MSIZE, MSIZE);

	printf ("Fixed Points:\n");
	printf ("     X           Y           Z\n");
	matrix_print (fixed, nfid, NDIM);

	printf ("Points to Align:\n");
	printf ("     X           Y           Z          error\n");
	for (i = 1; i <= nfid; i++) {
		dist = vector_dist (fixed[i], reslice[i]);
		for (j = 1; j <= NDIM; j++)
			fprintf (stdout, "%10.4f  ", reslice[i][j]);
		fprintf (stdout, "%10.4f\n", dist);
	}

	printf ("\nAligned Points:\n");
	printf ("     X           Y           Z          error\n");
	for (i = 1; i <= nfid; i++) {
		matrix_mult (mat, reslice[i], aligned, MSIZE, MSIZE, 1);
		dist = vector_dist (fixed[i], aligned);
		for (j = 1; j <= NDIM; j++)
			fprintf (stdout, "%10.4f  ", aligned[j]);
		fprintf (stdout, "%10.4f\n", dist);
	}

	free_matrix (fixed, 1, nfid, 1, MSIZE);
	free_matrix (reslice, 1, nfid, 1, MSIZE);
	free_matrix (x, 1, NPRM, 1, NPRM);
	free_vector (p, 1, NPRM);
	free_matrix (mat, 1, MSIZE, 1, MSIZE);
	free_vector (aligned, 1, MSIZE);
}
