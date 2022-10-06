/*  $Id$
    $Log$ */
/* ===============================================================================
 *  Module:         simplex.c
 *  Date:           19-Jul-95
 *  Author:         Tom Videen
 *  Description:
 *      Use simplex to find best fit to func in alignbyfids.
 *		Modification of amoeba.c in Numerical Recipes.
 * ===============================================================================
 */

#include <math.h>
#include <nrecipes/kr/nrutil.h>

#define NMAX 5000				 /* max number of function evaluations */

#define GET_PSUM 	for (j=1;j<=ndim;j++) {\
						for (sum=0.0,i=1; i<=mpts; i++) sum += p[i][j];\
						psum[j]=sum;}

#define SWAP(a,b)	{swap=(a); (a)=(b); (b)=swap;}

void            simplex (p, y, ndim, ftol, func, nfunc)
	float         **p;			 /* ndim + 1 points of simplex */
	float           y[];			 /* function values of points in simplex */
	float           ftol;			 /* tolerance for completion */
	float           (*func) ();		 /* function to minimize */
	int            *nfunc;			 /* number of function evaluations */
	int             ndim;			 /* number of parameters to fit */
{
	int             i, ihi, ilo, inhi, j, mpts = ndim + 1;
	float           rtol, sum, swap, ysave, ytry, *psum;

	psum = vector (1, ndim);
	*nfunc = 0;
	GET_PSUM;
	for (;;) {
		ilo = 1;

/* Determine which point is highest (worst), next highest, and lowest (best) */

		ihi = y[1] > y[2] ? (inhi = 2, 1) : (inhi = 1, 2);
		for (i = 1; i <= mpts; i++) {
			if (y[i] <= y[ilo])
				ilo = i;
			if (y[i] > y[ihi]) {
				inhi = ihi;
				ihi = i;
			} else if (y[i] > y[inhi] && i != ihi)
				inhi = i;
		}

/* Compute the fraction range from highest to lowest and return if satisfactory */

		rtol = 2.0 * fabs (y[ihi] - y[ilo]) / (fabs (y[ihi]) + fabs (y[ilo]));

/* If returning, put the best point and values in slot 1 */

		if (rtol < ftol) {
			SWAP (y[1], y[ilo]);
			for (i = 1; i <= ndim; i++)
				SWAP (p[1][i], p[ilo][i]);
			break;
		}
		if (*nfunc >= NMAX) {
			printf ("NMAX exceeded");
			break;
		}
/* Begin a new iteration */

		*nfunc += 2;
		ytry = simplextry (p, y, psum, ndim, func, ihi, -1.0);
		if (ytry <= y[ilo])
			ytry = simplextry (p, y, psum, ndim, func, ihi, 2.0);
		else if (ytry >= y[inhi]) {
			ysave = y[ihi];
			ytry = simplextry (p, y, psum, ndim, func, ihi, 0.5);
			if (ytry >= ysave) {
				for (i = 1; i <= mpts; i++) {
					if (i != ilo) {
						for (j = 1; j <= ndim; j++)
							p[i][j] = psum[j] = 0.5 * (p[i][j] + p[ilo][j]);
						y[i] = (*func) (psum);
					}
				}
				*nfunc += ndim;
				GET_PSUM;
			}
		} else
			--(*nfunc);
	}
	free_vector (psum, 1, ndim);
}

#undef SWAP
#undef GET_PSUM
#undef NMAX
