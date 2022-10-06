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

static char     rcsid[] = "$Header$";
float	vector_dist();
extern float  **fixed;
extern float  **reslice;
extern          nfid;

float           matdist (x)
	float           *x;
{
	float         **mat;
	float          *c;
	float           sumdif;
	int             i, j;

	mat = matrix (1, NP, 1, NP);
	c = vector (1, NP);
	matcreate (mat, x);

	for (sumdif = 0, i = 1; i <= nfid; i++) {
		matrix_mult (mat, reslice[i], c, NP, NP, 1);
		sumdif += vector_dist (fixed[i], c);
	}

	free_matrix (mat, 1, NP, 1, NP);
	free_vector (c, 1, NP);
	return (sumdif / (float) nfid);
}
