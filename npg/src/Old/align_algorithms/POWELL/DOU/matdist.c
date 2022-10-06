/*	$Log$
	$Id$ */
/*  ========================================================================
 *  Module:         matdist.c
 *  Date:           20-Jul-95
 *  Author:         Tom Videen
 *  Description:    Compute mean vector distance between transformed fiducial points.
 *  ========================================================================
 */

#include <math.h>
#include <nrecipes/kr/nr.h>
#include <nrecipes/kr/nrutil.h>
#include <petutil/matrixlib.h>

#define NP 4
#define DEGTORAD 3.14159265/180.

static char     rcsid[] = "$Header$";
extern double **fixed;
extern double **reslice;
extern          nfid;

double          matdist (x)
	double          x[];
{
	double        **mat, **tmp1, **tmp2, **tmp3;
	double        **tr, **rx, **ry, **rz;
	double         *c;
	double          dif, sqrdif, sumdif;
	int             i, j;

	mat = dmatrix (1, NP, 1, NP);
	tmp1 = dmatrix (1, NP, 1, NP);
	tmp2 = dmatrix (1, NP, 1, NP);
	tmp3 = dmatrix (1, NP, 1, NP);
	tr = dmatrix (1, NP, 1, NP);
	rx = dmatrix (1, NP, 1, NP);
	ry = dmatrix (1, NP, 1, NP);
	rz = dmatrix (1, NP, 1, NP);
	c = dvector (1, NP);

	for (i = 1; i <= NP; i++) {
		for (j = 1; j <= NP; j++) {
			mat[i][j] = (i == j) ? 1.0 : 0.0;
			tr[i][j] = rx[i][j] = ry[i][j] = rz[i][j] = mat[i][j];
		}
	}

/* Translation  & Rotation */

	tr[1][NP] = x[1];
	tr[2][NP] = x[2];
	tr[3][NP] = x[3];

	rx[2][2] = cos (DEGTORAD * x[4]);
	rx[2][3] = sin (DEGTORAD * x[4]);
	rx[3][2] = -sin (DEGTORAD * x[4]);
	rx[3][3] = cos (DEGTORAD * x[4]);

	ry[1][1] = cos (DEGTORAD * x[5]);
	ry[1][3] = -sin (DEGTORAD * x[5]);
	ry[3][1] = sin (DEGTORAD * x[5]);
	ry[3][3] = cos (DEGTORAD * x[5]);

	rz[1][1] = cos (DEGTORAD * x[6]);
	rz[1][2] = sin (DEGTORAD * x[6]);
	rz[2][1] = -sin (DEGTORAD * x[6]);
	rz[2][2] = cos (DEGTORAD * x[6]);

	dmatrix_mult (tr, mat, tmp1, NP, NP, NP);
	dmatrix_mult (rx, tmp1, tmp2, NP, NP, NP);
	dmatrix_mult (ry, tmp2, tmp3, NP, NP, NP);
	dmatrix_mult (rz, tmp3, mat, NP, NP, NP);

	sumdif = 0;
	for (i = 1; i <= nfid; i++) {
		sqrdif = 0;
		dmatrix_mult (mat, reslice[i], c, NP, NP, 1);
		for (j = 1; j <= NP; j++) {
			dif = fixed[i][j] - c[j];
			sqrdif += dif * dif;
		}
		sumdif += sqrt (sqrdif);
/*
		printf ("%9.2f", sqrt(sqrdif));
		if (i == 1) {
			printf ("%10.4f  %10.4f  %10.4f\n", fixed[1][1], fixed[1][2], fixed[1][3]);
			printf ("%10.4f  %10.4f  %10.4f\n", c[1], c[2], c[3]);
			dmatrix_print (mat, NP, NP);
			printf ("%10.4f  %10.4f  %10.4f\n", reslice[1][1], reslice[1][2], reslice[1][3]);
			printf ("%10.4f\n", (sumdif / (double) nfid));
		}
 */
	}
	return (sumdif / (double) nfid);
}
