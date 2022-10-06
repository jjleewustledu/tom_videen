/*	$Log$
	$Id$ */
/*  ========================================================================
 *  Module:         matcreate.c
 *  Date:           20-Jul-95
 *  Author:         Tom Videen
 *  Description:    Return a matrix created from 6 parameters:
 *					x[1]-x[3] = translation (x,y,z)
 *					x[4]-x[6] = rotation in degrees (x,y,z)
 *  ========================================================================
 */

#include <math.h>
#include <nrecipes/kr/nr.h>
#include <nrecipes/kr/nrutil.h>
#include <petutil/matrixlib.h>

#define NP 4
#define DEGTORAD 3.14159265/180.

static char     rcsid[] = "$Header$";

float           matcreate (mat, x)
	float         **mat;
	float           x[];
{
	float         **tmp1, **tmp2, **tmp3;
	float         **tr, **rx, **ry, **rz;
	int             i, j;

	tmp1 = matrix (1, NP, 1, NP);
	tmp2 = matrix (1, NP, 1, NP);
	tmp3 = matrix (1, NP, 1, NP);
	tr = matrix (1, NP, 1, NP);
	rx = matrix (1, NP, 1, NP);
	ry = matrix (1, NP, 1, NP);
	rz = matrix (1, NP, 1, NP);

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

	matrix_mult (tr, mat, tmp1, NP, NP, NP);
	matrix_mult (rx, tmp1, tmp2, NP, NP, NP);
	matrix_mult (ry, tmp2, tmp3, NP, NP, NP);
	matrix_mult (rz, tmp3, mat, NP, NP, NP);
}
