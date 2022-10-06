#include "curvature.h"


#include <stdlib.h>
#include <math.h>
#include "fftw3.h"


void            printDeltaPlan (deltaPlan p)
{
	printf ("-- deltaPlan at %p\n", p);
	printf ("%30s : %p\n", "forward", p->forward);
	printf ("%30s : %p\n", "backward", p->backward);
	printf ("%30s : %p\n", "in", p->in);
	printf ("%30s : %p\n", "out", p->out);
	printf ("%30s : %g\n", "fftw_normfac", p->fftw_normfac);
	printf ("%30s : %d\n", "dim", p->dim);
	printf ("%30s : %d\n", "m1", p->m1);
	printf ("%30s : %d\n", "m2", p->m2);
	printf ("%30s : %d\n", "m3", p->m3);
	printf ("%30s : %d\n", "m", p->m);
	printf ("%30s : %g\n", "h1", p->h1);
	printf ("%30s : %g\n", "h2", p->h2);
	printf ("%30s : %g\n", "h3", p->h3);
	printf ("%30s : %p\n", "lambda1", p->lambda1);
	printf ("%30s : %p\n", "lambda2", p->lambda2);
	printf ("%30s : %p\n", "lambda3", p->lambda3);
	printf ("%30s : %s\n", "bc", p->bc == NEUMANN ? "NEUMANN" : "DIRICHLET");
	printf ("%30s : %s\n", "gs", p->gs == MIDPOINT ? "MIDPOINT" : "GRIDPOINT");
}

deltaPlan       createDeltaPlan2D (int m1,
				                   int m2,
				                   double h1,
				                   double h2,
				                   boundary_t bc,
				                   gridsymmetry_t gs)
{


	int             k;
	deltaPlan       plan = (deltaPlan) malloc (sizeof (deltaPlan_t));

	plan->dim = 2;
	plan->m1 = m1;
	plan->m2 = m2;
	plan->m3 = 1;
	plan->m = m1 * m2;
	plan->h1 = h1;
	plan->h2 = h2;
	plan->h3 = 0.0;
	plan->bc = bc;
	plan->gs = gs;
	plan->lambda1 = (double *) fftw_malloc (sizeof (double) * m1);
	plan->lambda2 = (double *) fftw_malloc (sizeof (double) * m2);
	plan->lambda3 = NULL;
	plan->in = (double *) fftw_malloc (sizeof (double) * m1 * m2);
	plan->out = (double *) fftw_malloc (sizeof (double) * m1 * m2);


	switch (gs) {
	case MIDPOINT:
		switch (bc) {
		case NEUMANN:			 /* midpoint symmetry + neumann bc; forward=dct2,
						  * backward=dct3 */
			plan->fftw_normfac = 1.0 / ((double) (2 * m1 * 2 * m2));
			plan->forward = fftw_plan_r2r_2d (m1, m2, plan->in, plan->out, FFTW_REDFT10, FFTW_REDFT10, FFTW_ESTIMATE);
			plan->backward = fftw_plan_r2r_2d (m1, m2, plan->in, plan->out, FFTW_REDFT01, FFTW_REDFT01, FFTW_ESTIMATE);
			for (k = 0; k < m1; ++k)
				plan->lambda1[k] = -(2.0 - 2.0 * cos (((double) k) * PI / ((double) m1)));
			for (k = 0; k < m2; ++k)
				plan->lambda2[k] = -(2.0 - 2.0 * cos (((double) k) * PI / ((double) m2)));
			break;

		case DIRICHLET:		 /* midpoint symmetry + dirichlet bc forward=dst2,
						  * backward=dst3 */
			plan->fftw_normfac = 1.0 / ((double) (2 * m1 * 2 * m2));
			plan->forward = fftw_plan_r2r_2d (m1, m2, plan->in, plan->out, FFTW_RODFT10, FFTW_RODFT10, FFTW_ESTIMATE);
			plan->backward = fftw_plan_r2r_2d (m1, m2, plan->in, plan->out, FFTW_RODFT01, FFTW_RODFT01, FFTW_ESTIMATE);
			for (k = 0; k < m1; ++k)
				plan->lambda1[k] = -(2.0 - 2.0 * cos (((double) (k + 1)) * PI / ((double) m1)));
			for (k = 0; k < m2; ++k)
				plan->lambda2[k] = -(2.0 - 2.0 * cos (((double) (k + 1)) * PI / ((double) m2)));
			break;
		}

	case GRIDPOINT:
		switch (bc) {

		case NEUMANN:			 /* gridpoint symmetry + neumann bc
						  * forward=backward=dct1 */
			plan->fftw_normfac = 1.0 / ((double) (2 * (m1 - 1) * 2 * (m2 - 1)));
			plan->forward = fftw_plan_r2r_2d (m1, m2, plan->in, plan->out, FFTW_REDFT00, FFTW_REDFT00, FFTW_ESTIMATE);
			plan->backward = fftw_plan_r2r_2d (m1, m2, plan->in, plan->out, FFTW_REDFT00, FFTW_REDFT00, FFTW_ESTIMATE);
			for (k = 0; k < m1; ++k)
				plan->lambda1[k] = -(2.0 - 2.0 * cos (((double) k) * PI / ((double) (m1 - 1))));
			for (k = 0; k < m2; ++k)
				plan->lambda2[k] = -(2.0 - 2.0 * cos (((double) k) * PI / ((double) (m2 - 1))));
			break;

		case DIRICHLET:		 /* gridpoint symmetry + dirichlet bc
						  * forward=backward=dst1 */
			plan->fftw_normfac = 1.0 / ((double) (2 * (m1 + 1) * 2 * (m2 + 1)));
			plan->forward = fftw_plan_r2r_2d (m1, m2, plan->in, plan->out, FFTW_RODFT00, FFTW_RODFT00, FFTW_ESTIMATE);
			plan->backward = fftw_plan_r2r_2d (m1, m2, plan->in, plan->out, FFTW_RODFT00, FFTW_RODFT00, FFTW_ESTIMATE);
			for (k = 0; k < m1; ++k)
				plan->lambda1[k] = -(2.0 - 2.0 * cos (((double) (k + 1)) * PI / ((double) (m1 + 1))));
			for (k = 0; k < m2; ++k)
				plan->lambda2[k] = -(2.0 - 2.0 * cos (((double) (k + 1)) * PI / ((double) (m2 + 1))));
			break;
		}
	}

	return plan;

}



deltaPlan       createDeltaPlan3D (int m1,
				                   int m2,
				                   int m3,
				                   double h1,
				                   double h2,
				                   double h3,
				                   boundary_t bc,
				                   gridsymmetry_t gs)
{

	int             k;
	deltaPlan       plan = (deltaPlan) malloc (sizeof (deltaPlan_t));

	plan->dim = 3;
	plan->m1 = m1;
	plan->m2 = m2;
	plan->m3 = m3;
	plan->m = m1 * m2 * m3;
	plan->h1 = h1;
	plan->h2 = h2;
	plan->h3 = h3;
	plan->bc = bc;
	plan->gs = gs;
	plan->lambda1 = (double *) fftw_malloc (sizeof (double) * m1);
	plan->lambda2 = (double *) fftw_malloc (sizeof (double) * m2);
	plan->lambda3 = (double *) fftw_malloc (sizeof (double) * m3);
	plan->in = (double *) fftw_malloc (sizeof (double) * m1 * m2 * m3);
	plan->out = (double *) fftw_malloc (sizeof (double) * m1 * m2 * m3);


	switch (gs) {
	case MIDPOINT:
		switch (bc) {
		case NEUMANN:			 /* midpoint symmetry + neumann bc; forward=dct2,
						  * backward=dct3 */
			plan->fftw_normfac = 1.0 / ((double) (2 * m1 * 2 * m2 * 2 * m3));
			plan->forward = fftw_plan_r2r_3d (m1, m2, m3, plan->in, plan->out, FFTW_REDFT10, FFTW_REDFT10, FFTW_REDFT10, FFTW_ESTIMATE);
			plan->backward = fftw_plan_r2r_3d (m1, m2, m3, plan->in, plan->out, FFTW_REDFT01, FFTW_REDFT01, FFTW_REDFT01, FFTW_ESTIMATE);
			for (k = 0; k < m1; ++k)
				plan->lambda1[k] = -(2.0 - 2.0 * cos (((double) k) * PI / ((double) m1)));
			for (k = 0; k < m2; ++k)
				plan->lambda2[k] = -(2.0 - 2.0 * cos (((double) k) * PI / ((double) m2)));
			for (k = 0; k < m3; ++k)
				plan->lambda3[k] = -(2.0 - 2.0 * cos (((double) k) * PI / ((double) m3)));
			break;

		case DIRICHLET:		 /* midpoint symmetry + dirichlet bc forward=dst2,
						  * backward=dst3 */
			plan->fftw_normfac = 1.0 / ((double) (2 * m1 * 2 * m2 * 2 * m3));
			plan->forward = fftw_plan_r2r_3d (m1, m2, m3, plan->in, plan->out, FFTW_RODFT10, FFTW_RODFT10, FFTW_RODFT10, FFTW_ESTIMATE);
			plan->backward = fftw_plan_r2r_3d (m1, m2, m3, plan->in, plan->out, FFTW_RODFT01, FFTW_RODFT01, FFTW_RODFT01, FFTW_ESTIMATE);
			for (k = 0; k < m1; ++k)
				plan->lambda1[k] = -(2.0 - 2.0 * cos (((double) (k + 1)) * PI / ((double) m1)));
			for (k = 0; k < m2; ++k)
				plan->lambda2[k] = -(2.0 - 2.0 * cos (((double) (k + 1)) * PI / ((double) m2)));
			for (k = 0; k < m3; ++k)
				plan->lambda3[k] = -(2.0 - 2.0 * cos (((double) (k + 1)) * PI / ((double) m3)));
			break;
		}

	case GRIDPOINT:
		switch (bc) {

		case NEUMANN:			 /* gridpoint symmetry + neumann bc
						  * forward=backward=dct1 */
			plan->fftw_normfac = 1.0 / ((double) (2 * (m1 - 1) * 2 * (m2 - 1) * 2 * (m3 - 1)));
			plan->forward = fftw_plan_r2r_3d (m1, m2, m3, plan->in, plan->out, FFTW_REDFT00, FFTW_REDFT00, FFTW_REDFT00, FFTW_ESTIMATE);
			plan->backward = fftw_plan_r2r_3d (m1, m2, m3, plan->in, plan->out, FFTW_REDFT00, FFTW_REDFT00, FFTW_REDFT00, FFTW_ESTIMATE);
			for (k = 0; k < m1; ++k)
				plan->lambda1[k] = -(2.0 - 2.0 * cos (((double) k) * PI / ((double) (m1 - 1))));
			for (k = 0; k < m2; ++k)
				plan->lambda2[k] = -(2.0 - 2.0 * cos (((double) k) * PI / ((double) (m2 - 1))));
			for (k = 0; k < m3; ++k)
				plan->lambda3[k] = -(2.0 - 2.0 * cos (((double) k) * PI / ((double) (m3 - 1))));
			break;

		case DIRICHLET:		 /* gridpoint symmetry + dirichlet bc
						  * forward=backward=dst1 */
			plan->fftw_normfac = 1.0 / ((double) (2 * (m1 + 1) * 2 * (m2 + 1) * 2 * (m3 + 1)));
			plan->forward = fftw_plan_r2r_3d (m1, m2, m3, plan->in, plan->out, FFTW_RODFT00, FFTW_RODFT00, FFTW_RODFT00, FFTW_ESTIMATE);
			plan->backward = fftw_plan_r2r_3d (m1, m2, m3, plan->in, plan->out, FFTW_RODFT00, FFTW_RODFT00, FFTW_RODFT00, FFTW_ESTIMATE);
			for (k = 0; k < m1; ++k)
				plan->lambda1[k] = -(2.0 - 2.0 * cos (((double) (k + 1)) * PI / ((double) (m1 + 1))));
			for (k = 0; k < m2; ++k)
				plan->lambda2[k] = -(2.0 - 2.0 * cos (((double) (k + 1)) * PI / ((double) (m2 + 1))));
			for (k = 0; k < m3; ++k)
				plan->lambda3[k] = -(2.0 - 2.0 * cos (((double) (k + 1)) * PI / ((double) (m3 + 1))));
			break;
		}
	}

	return plan;

}


void            deleteDeltaPlan (deltaPlan plan)
{
	if (plan != NULL) {
		fftw_destroy_plan (plan->forward);
		fftw_destroy_plan (plan->backward);
		fftw_free (plan->in);
		fftw_free (plan->out);
		fftw_free (plan->lambda1);
		fftw_free (plan->lambda2);
		fftw_free (plan->lambda3);
		free (plan);
	}
}




/* compute (a*I + b*Delta) in = out */
void            evalDelta (deltaPlan plan,
			                   double a,
			                   double b,
			                   double *in,
			                   double *out)
{
	int             k, k1, k2, k3;
	double          lambda;
	for (k = 0; k < plan->m; ++k)
		plan->in[k] = in[k];

	fftw_execute (plan->forward);

	switch (plan->dim) {
	case 2:
		k = 0;
		for (k1 = 0; k1 < plan->m1; ++k1)
			for (k2 = 0; k2 < plan->m2; ++k2) {
				lambda = a + b * (plan->h1 * plan->lambda1[k1] +
						  plan->h2 * plan->lambda2[k2]);
				plan->in[k] = lambda * plan->out[k];
				k++;
			}
		break;

	case 3:
		k = 0;
		for (k1 = 0; k1 < plan->m1; ++k1)
			for (k2 = 0; k2 < plan->m2; ++k2)
				for (k3 = 0; k3 < plan->m3; ++k3) {
					lambda = a + b * (plan->h1 * plan->lambda1[k1] +
							  plan->h2 * plan->lambda2[k2] +
							  plan->h3 * plan->lambda3[k3]);
					plan->in[k] = lambda * plan->out[k];
					k++;
				}
		break;

	default:
		printf ("%s %d (build %s %s) -- evalDelta\n");
		printf ("\tCannot handle %d dimensions.\n");
		printf ("\tExiting on error\n");
		exit (-1);
	}

	fftw_execute (plan->backward);

	for (k = 0; k < plan->m; ++k)
		out[k] = plan->fftw_normfac * plan->out[k];
}



/* solve (a*I + b*Delta) out = in */
void            solveDelta (deltaPlan plan,
			                    double a,
			                    double b,
			                    double *in,
			                    double *out)
{
	int             k, k1, k2, k3;
	double          lambda;

	for (k = 0; k < plan->m; ++k)
		plan->in[k] = in[k];

	fftw_execute (plan->forward);

	switch (plan->dim) {
	case 2:
		k = 0;
		for (k1 = 0; k1 < plan->m1; ++k1)
			for (k2 = 0; k2 < plan->m2; ++k2) {
				lambda = a + b * (plan->h1 * plan->lambda1[k1] +
						  plan->h2 * plan->lambda2[k2]);
				plan->in[k] = (lambda == 0.0) ? 0.0 : 1.0 / lambda * plan->out[k];
				k++;
			}
		break;

	case 3:
		k = 0;
		for (k1 = 0; k1 < plan->m1; ++k1)
			for (k2 = 0; k2 < plan->m2; ++k2)
				for (k3 = 0; k3 < plan->m3; ++k3) {
					lambda = a + b * (plan->h1 * plan->lambda1[k1] +
							  plan->h2 * plan->lambda2[k2] +
							  plan->h3 * plan->lambda3[k3]);
					plan->in[k] = (lambda == 0.0) ? 0.0 : 1.0 / lambda * plan->out[k];
					k++;
				}
		break;

	default:
		printf ("%s %d (build %s %s) -- evalDelta\n");
		printf ("\tCannot handle %d dimensions.\n");
		printf ("\tExiting on error\n");
		exit (-1);
	}

	fftw_execute (plan->backward);

	for (k = 0; k < plan->m; ++k)
		out[k] = plan->fftw_normfac * plan->out[k];
}








/* ---------------------------------------- */
/* ---------------------------------------- */
/* ---------------------------------------- */
/* ---------------------------------------- */
/* ---------------------------------------- */

/* compute (a*I + b*Delta^2) in = out */
void            evalCurvature (deltaPlan plan,
			                       double a,
			                       double b,
			                       double *in,
			                       double *out)
{
	int             k, k1, k2, k3;
	double          lambda;

	for (k = 0; k < plan->m; ++k)
		plan->in[k] = in[k];

	fftw_execute (plan->forward);

	switch (plan->dim) {
	case 2:
		k = 0;
		for (k1 = 0; k1 < plan->m1; ++k1)
			for (k2 = 0; k2 < plan->m2; ++k2) {
				lambda = a + b * ((plan->h1 * plan->lambda1[k1] +
						   plan->h2 * plan->lambda2[k2])
						  *
						  (plan->h1 * plan->lambda1[k1] +
						   plan->h2 * plan->lambda2[k2]));
				plan->in[k] = lambda * plan->out[k];
				k++;
			}
		break;

	case 3:
		k = 0;
		for (k1 = 0; k1 < plan->m1; ++k1)
			for (k2 = 0; k2 < plan->m2; ++k2)
				for (k3 = 0; k3 < plan->m3; ++k3) {
					lambda = a + b * ((plan->h1 * plan->lambda1[k1] +
							   plan->h2 * plan->lambda2[k2] +
							   plan->h3 * plan->lambda3[k3])
							  *
							  (plan->h1 * plan->lambda1[k1] +
							   plan->h2 * plan->lambda2[k2] +
							   plan->h3 * plan->lambda3[k3]));
					plan->in[k] = lambda * plan->out[k];
					k++;
				}
		break;

	default:
		printf ("%s %d (build %s %s) -- evalDelta\n");
		printf ("\tCannot handle %d dimensions.\n");
		printf ("\tExiting on error\n");
		exit (-1);
	}

	fftw_execute (plan->backward);

	for (k = 0; k < plan->m; ++k)
		out[k] = plan->fftw_normfac * plan->out[k];
}



/* solve (a*I + b*Delta^2) out = in */
void            solveCurvature (deltaPlan plan,
				                double a,
				                double b,
				                double *in,
				                double *out)
{
	int             k, k1, k2, k3;
	double          lambda;

	for (k = 0; k < plan->m; ++k)
		plan->in[k] = in[k];

	fftw_execute (plan->forward);

	switch (plan->dim) {
	case 2:
		k = 0;
		for (k1 = 0; k1 < plan->m1; ++k1)
			for (k2 = 0; k2 < plan->m2; ++k2) {
				lambda = a + b * ((plan->h1 * plan->lambda1[k1] +
						   plan->h2 * plan->lambda2[k2])
						  *
						  (plan->h1 * plan->lambda1[k1] +
						   plan->h2 * plan->lambda2[k2]));
				plan->in[k] = (lambda == 0.0) ? 0.0 : 1.0 / lambda * plan->out[k];
				k++;
			}
		break;

	case 3:
		k = 0;
		for (k1 = 0; k1 < plan->m1; ++k1)
			for (k2 = 0; k2 < plan->m2; ++k2)
				for (k3 = 0; k3 < plan->m3; ++k3) {
					lambda = a + b * ((plan->h1 * plan->lambda1[k1] +
							   plan->h2 * plan->lambda2[k2] +
							   plan->h3 * plan->lambda3[k3])
							  *
							  (plan->h1 * plan->lambda1[k1] +
							   plan->h2 * plan->lambda2[k2] +
							   plan->h3 * plan->lambda3[k3]));
					plan->in[k] = (lambda == 0.0) ? 0.0 : 1.0 / lambda * plan->out[k];
					k++;
				}
		break;

	default:
		printf ("%s %d (build %s %s) -- evalDelta\n");
		printf ("\tCannot handle %d dimensions.\n");
		printf ("\tExiting on error\n");
		exit (-1);
	}

	fftw_execute (plan->backward);

	for (k = 0; k < plan->m; ++k)
		out[k] = plan->fftw_normfac * plan->out[k];
}
