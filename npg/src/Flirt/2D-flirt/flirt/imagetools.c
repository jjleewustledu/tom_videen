#include <imagetools.h>
#include <stdlib.h>
#include <math.h>

/*
 Compute gradient of T using central finite differences; at the boundarys
 DIRICHLET or NEUMANN are used,
    d1T[j1][j2] = 1/2* ( T[j1+1][j2] - T[j1-1][j2])
    d2T[j1][j2] = 1/2* ( T[j1][j2+1] - T[j1][j2-1])
*/
void            computeFDGrad2D (array2D T,
				                 size_t m1,
				                 size_t m2,
				                 double h1,
				                 double h2,
				                 boundary_t bc,
				                 array3D dT)
{
	size_t          j1, j2;
	double          fac;

	/* compute Tner gradient without respect to BC */

	/* Gradient T x1 */
	fac = 1 / (2 * h1);
	for (j1 = 1; j1 < (m1 - 1); j1++) {
		for (j2 = 0; j2 < m2; j2++) {
			dT[0][j1][j2] = fac * (T[j1 + 1][j2] - T[j1 - 1][j2]);
		}
	}
	/* Gradient T x2 */
	fac = 1 / (2 * h2);
	for (j1 = 0; j1 < m1; j1++) {
		for (j2 = 1; j2 < (m2 - 1); j2++) {
			dT[1][j1][j2] = fac * (T[j1][j2 + 1] - T[j1][j2 - 1]);
		}
	}
	/* compute gradient on image boundarys with respect to BC */
	switch (bc) {
	case NEUMANN:				 /* Neumann 0 BC */
		{
			/* T x1 Direction */
			fac = 1 / (2 * h1);
			for (j2 = 0; j2 < m2; j2++) {
				dT[0][0][j2] = fac * (T[1][j2] - T[0][j2]);
				dT[0][m1 - 1][j2] = fac * (T[m1 - 1][j2] - T[m1 - 2][j2]);
			}
			/* T x2 Direction */
			fac = 1 / (2 * h2);
			for (j1 = 0; j1 < m1; j1++) {
				dT[1][j1][0] = fac * (T[j1][1] - T[j1][0]);
				dT[1][j1][m2 - 1] = fac * (T[j1][m2 - 1] - T[j1][m2 - 2]);
			}

			break;
		}				 /* end of case NEUMANN */

	case DIRICHLET:			 /* Dirichlet 0 BC */
		{
			/* T x1 Direction */
			fac = 1 / (h1);
			for (j2 = 0; j2 < m2; j2++) {
				dT[0][0][j2] = fac * (T[1][j2]);
				dT[0][m1 - 1][j2] = fac * (-T[m1 - 2][j2]);
			}

			/* T x2 Direction */
			fac = 1 / (h2);
			for (j1 = 0; j1 < m1; j1++) {
				dT[1][j1][0] = fac * (T[j1][1]);
				dT[1][j1][m1 - 1] = fac * (-T[j1][m2 - 2]);
			}

			break;
		}				 /* end of case DIRICHLET */

	default:{
		}

	}					 /* end of switch bc */
	/* printf("\nend\n"); */
}					 /* end of computeFDGrad2D() */


void            evalDisplacementLinear2D (array2D T,
					                  size_t m1,
					                  size_t m2,
					                  array2D U1,
					                  array2D U2,
					                  array2D Tphi)
{

	size_t          j1, j2, k, i1, i2, index;

	int             I1[2];
	int             I2[2];
	double          w1[2];
	double          w2[2];
	double          X, Y;

	for (j1 = 0; j1 < m1; j1++) {
		for (j2 = 0; j2 < m2; j2++) {
			X = j1 + U1[j1][j2];
			Y = j2 + U2[j1][j2];

			w1[1] = X - floor (X);
			w1[0] = 1.0 - w1[1];

			w2[1] = Y - floor (Y);
			w2[0] = 1.0 - w2[1];



			index = (int) floor (X);
			for (k = 0; k < 2; ++k) {
				if ((index < 0) || (index > (m1 - 1))) {
					I1[k] = 0;
					w1[k] = 0.0;
				} else {
					I1[k] = index;
				}
				index++;
			}

			index = (int) floor (Y);
			for (k = 0; k < 2; ++k) {
				if ((index < 0) || (index > (m2 - 1))) {
					I2[k] = 0;
					w2[k] = 0.0;
				} else {
					I2[k] = index;
				}
				index++;
			}

			Tphi[j1][j2] = 0.0;
			for (i1 = 0; i1 < 2; ++i1)
				for (i2 = 0; i2 < 2; ++i2)
					Tphi[j1][j2] += w1[i1] * w2[i2] * T[I1[i1]][I2[i2]];

		}
	}
}						 /* end of evalDisplacementLinear2D() */
