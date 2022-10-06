// (c) SW, 2005
#include <elastic.h>
#include <arrays.h>
#include <stdio.h>
#include <fftw3.h>

/* --------------------------------------------------------------------- */
void            initElasticMatrix2D (size_t m1,
				                     size_t m2,
				                     double mu,
				                     double lambda,
				                     double h1,
				                     double h2,
				                     boundary_t bc,
				                     elasticMatrix2D * pMatrix)
{

	size_t          k1, k2;
	double          ak1, ak2, ck1, ck2, sk1, sk2;
	double          d11, d12, d22, detD;
	double          lp2m = lambda + 2 * mu;
	double          lpm = lambda + mu;
	double          m2lp3m = -2 * (lambda + 3 * mu);
	array3D         invD;

	elasticMatrix2D M;
	M = (elasticMatrix2D) malloc (sizeof (elasticMatrix2D_t));
	/** SB */
	M->data = createTreeStructure3D (3, m1, m2, (double *) calloc (3 * m2 * m1, sizeof (array_t)));
	/* \SB */

	M->len = m1 * m2 * 3;

	invD = M->data;

	if (bc == PERIODIC) {
		/* create inverse Matrix -------------------------------------------------- */
		for (k1 = 0; k1 < m1; k1++) {
			for (k2 = 0; k2 < m2; k2++) {
				ak1 = 2 * PI * k1 / m1;
				ak2 = 2 * PI * k2 / m2;

				ck1 = 2 * cos (ak1);
				ck2 = 2 * cos (ak2);
				sk1 = sin (ak1);
				sk2 = sin (ak2);

				d11 = (m2lp3m + lp2m * ck1 + mu * ck2) / (h1 * h1);
				d12 = -lpm * sk1 * sk2 / (h1 * h2);
				d22 = (m2lp3m + mu * ck1 + lp2m * ck2) / (h2 * h2);

				/*
				 * calculate determinant
				 * --------------------------------------------
				 */
				detD = d11 * d22 - d12 * d12;

				if (fabs (detD) < 1e-15) {
					/* numerically zero */
					invD[0][k1][k2] = invD[1][k1][k2] = invD[2][k1][k2] = 0;
				} else {
					invD[0][k1][k2] = d22 / detD;
					invD[1][k1][k2] = -d12 / detD;
					invD[2][k1][k2] = d11 / detD;
				}
			}
		}

		*(pMatrix) = M;
	} else {
		printf ("Elastic solver currently only implemented for periodic boundary conditions!");
		*(pMatrix) = NULL;
	}
}

/* --------------------------------------------------------------------- */
void            initElasticPlan2D (size_t m1,
				                   size_t m2,
				                   double alpha,
				                   double mu,
				                   double lambda,
				                   double h1,
				                   double h2,
				                   boundary_t bc,
				                   elasticPlan * pPlan)
{

	elasticPlan     EP;
	EP = (elasticPlan) malloc (sizeof (elasticPlan_t));

	initElasticMatrix2D (m1, m2, mu, lambda, h1, h2, bc, &(EP->M2D));

	EP->in = (fftw_complex *) fftw_malloc (sizeof (fftw_complex) * m1 * m2);
	EP->out = (fftw_complex *) fftw_malloc (sizeof (fftw_complex) * m1 * m2);
	EP->pForw = fftw_plan_dft_2d (m1, m2, EP->in, EP->out, FFTW_FORWARD, FFTW_MEASURE);
	EP->pBackw = fftw_plan_dft_2d (m1, m2, EP->in, EP->out, FFTW_BACKWARD, FFTW_MEASURE);

	EP->alpha = alpha;
	EP->mu = mu;
	EP->lambda = lambda;
	EP->dim = 2;
	EP->m1 = m1;
	EP->m2 = m2;
	EP->m3 = 1;
	EP->h1 = h1;
	EP->h2 = h2;
	EP->h3 = 0;
	EP->bc = bc;

	*(pPlan) = EP;
}

/* --------------------------------------------------------------------- */
void            printElasticPlan (const elasticPlan M)
{

	printf ("%s %d (Build %s %s) -- \n", __FILE__, __LINE__, __DATE__, __TIME__);

	printf ("-- elasticPlan at %p\n", M);
	printf ("%30s : %p\n", "M->M", M->M2D);
	printf ("%30s : %p\n", "M->pForw", M->pForw);
	printf ("%30s : %p\n", "M->pBackw", M->pBackw);
	printf ("%30s : %p\n", "M->in", M->in);
	printf ("%30s : %p\n", "M->out", M->out);
	printf ("%30s : %g\n", "M->alpha", M->alpha);
	printf ("%30s : %g\n", "M->mu", M->mu);
	printf ("%30s : %g\n", "M->lambda", M->lambda);
	printf ("%30s : %d\n", "M->dim", M->dim);
	printf ("%30s : %g\n", "M->h1", M->h1);
	printf ("%30s : %g\n", "M->h2", M->h2);
	printf ("%30s : %g\n", "M->h3", M->h3);
	printf ("%30s : %d\n", "M->m1", M->m1);
	printf ("%30s : %d\n", "M->m2", M->m2);
	printf ("%30s : %d\n", "M->m3", M->m3);
	printf ("%30s : %d\n", "M->bc", M->bc);
}

/* --------------------------------------------------------------------- */
void            solveElastic2D (elasticPlan EP,
				                double ***in,
				                double ***out)
{
	int             k1;
	int             m1 = EP->m1;
	int             m2 = EP->m2;
	int             m12 = m1 * m2;
	double         *in1 = in[0][0];
	double         *in2 = in[1][0];
	double          invAlpha = 1. / (EP->alpha);
	array3D         invD = EP->M2D->data;
	fftw_complex   *fftIn1, *fftIn2;
	double          maxX1, maxX2;
	maxX1 = maxX2 = 0;

	fftIn1 = (fftw_complex *) fftw_malloc (sizeof (fftw_complex) * m12);
	fftIn2 = (fftw_complex *) fftw_malloc (sizeof (fftw_complex) * m12);

	/* fftIn1=fft2(in1) */
	for (k1 = 0; k1 < m12; k1++) {
		EP->in[k1][0] = (-invAlpha) * in1[k1];
		EP->in[k1][1] = 0;
	}
	fftw_execute (EP->pForw);
	for (k1 = 0; k1 < m12; k1++) {
		fftIn1[k1][0] = EP->out[k1][0];
		fftIn1[k1][1] = EP->out[k1][1];
	}

	/* fftIn2=fft2(in2) */
	for (k1 = 0; k1 < m12; k1++) {
		EP->in[k1][0] = (-invAlpha) * in2[k1];
		EP->in[k1][1] = 0;
	}
	fftw_execute (EP->pForw);
	for (k1 = 0; k1 < m12; k1++) {
		fftIn2[k1][0] = EP->out[k1][0];
		fftIn2[k1][1] = EP->out[k1][1];
	}


	/* calculate du1 */
	/* du1 = invD11.*fft2(in1) + invD12.*fft2(in2) */
	for (k1 = 0; k1 < m12; k1++) {
		EP->in[k1][0] = (invD[0][0][k1] * fftIn1[k1][0] + invD[1][0][k1] * fftIn2[k1][0]);	/* real */
		EP->in[k1][1] = (invD[0][0][k1] * fftIn1[k1][1] + invD[1][0][k1] * fftIn2[k1][1]);	/* imag */
	}
	fftw_execute (EP->pBackw);
	for (k1 = 0; k1 < m12; k1++) {
		out[0][0][k1] = EP->out[k1][0];
		maxX1 = maxAbs (maxX1, out[0][0][k1]);
	}

	/* calculate du2 */
	/* du2 = invD12.*fft2(in1) + invD22.*fft2(in2) */
	for (k1 = 0; k1 < m12; k1++) {
		EP->in[k1][0] = (invD[1][0][k1] * fftIn1[k1][0] + invD[2][0][k1] * fftIn2[k1][0]);	/* real */
		EP->in[k1][1] = (invD[1][0][k1] * fftIn1[k1][1] + invD[2][0][k1] * fftIn2[k1][1]);	/* imag */
	}
	fftw_execute (EP->pBackw);
	for (k1 = 0; k1 < m12; k1++) {
		out[1][0][k1] = EP->out[k1][0];
		maxX2 = maxAbs (maxX2, out[1][0][k1]);
	}

	fftw_free (fftIn1);
	fftw_free (fftIn2);

	printf ("\tmax(abs(dU1)) = %f, max(abs(dU2)) = %f\n", maxX1, maxX2);
}

/*==============================================
/* 3d-Fall
/*==============================================

/* --------------------------------------------------------------------- */
void            initElasticMatrix3D (size_t m1,
				                     size_t m2,
				                     size_t m3,
				                     double mu,
				                     double lambda,
				                     double h1,
				                     double h2,
				                     double h3,
				                     boundary_t bc,
				                     elasticMatrix3D * pMatrix)
{

	size_t          k1, k2, k3;
	double          ak1, ak2, ak3, ck1, ck2, ck3, sk1, sk2, sk3;
	double          d11, d12, d13, d22, d23, d33, detD;

	double          m2lp4m = -2 * (lambda + 3 * mu);
	double          lp2m = lambda + 2 * mu;
	double          lpm = lambda + mu;
	array4D         invD;

	elasticMatrix3D M;
	M = (elasticMatrix3D) malloc (sizeof (elasticMatrix3D_t));
	/** SB */
	M->data = createTreeStructure4D (6, m1, m2, m3, (double *) calloc (6 * m3 * m2 * m1, sizeof (array_t)));
	/* \SB */

	M->len = m1 * m2 * m3 * 6;

	invD = M->data;

	if (bc == PERIODIC) {
		/* create inverse Matrix -------------------------------------------------- */
		for (k1 = 0; k1 < m1; k1++) {
			for (k2 = 0; k2 < m2; k2++) {
				for (k3 = 0; k3 < m3; k3++) {
					ak1 = 2 * PI * k1 / m1;
					ak2 = 2 * PI * k2 / m2;
					ak3 = 2 * PI * k3 / m3;

					ck1 = 2 * cos (ak1);
					ck2 = 2 * cos (ak2);
					ck3 = 2 * cos (ak3);

					sk1 = sin (ak1);
					sk2 = sin (ak2);
					sk3 = sin (ak3);

					d11 = (m2lp4m + lp2m * ck1 + mu * (ck2 + ck3)) / (h1 * h1);
					d12 = -lpm * sk1 * sk2 / (h1 * h2);
					d13 = -lpm * sk1 * sk3 / (h1 * h3);
					d22 = (m2lp4m + mu * (ck1 + ck2) + lp2m * ck3) / (h2 * h2);
					d23 = -lpm * sk2 * sk3 / (h2 * h3);
					d33 = (m2lp4m + mu * (ck1 + ck3) + lp2m * ck2) / (h3 * h3);

					/*
					 * calculate determinant
					 * --------------------------------------------
					 */
					detD = d11 * d22 * d33 - d11 * d23 * d23 - d12 * d12 * d33 + d12 * d13 * d23 + d12 * d13 * d23 - d13 * d13 * d22;

					if (fabs (detD) < 1e-15) {
						/* numerically zero */

						/*
						 * invD[0] = D11+ invD[1] = D12+ invD[2] = D13+
						 * invD[3] = D22+ invD[4] = D23+ invD[5] = D33+
						 */
						invD[0][k1][k2][k3] = 0;
						invD[1][k1][k2][k3] = 0;
						invD[2][k1][k2][k3] = 0;
						invD[3][k1][k2][k3] = 0;
						invD[4][k1][k2][k3] = 0;
						invD[5][k1][k2][k3] = 0;
					} else {
						invD[0][k1][k2][k3] = (d22 * d33 - d23 * d23) / detD;
						invD[1][k1][k2][k3] = (d13 * d23 - d12 * d33) / detD;
						invD[2][k1][k2][k3] = (d12 * d23 - d13 * d22) / detD;
						invD[3][k1][k2][k3] = (d11 * d33 - d13 * d13) / detD;
						invD[4][k1][k2][k3] = (d12 * d13 - d11 * d23) / detD;
						invD[5][k1][k2][k3] = (d11 * d22 - d12 * d12) / detD;
					}
				}
			}
		}

		*(pMatrix) = M;
	} else {
		printf ("Elastic solver currently only implemented for periodic boundary conditions!");
		*(pMatrix) = NULL;
	}
}

/* --------------------------------------------------------------------- */
void            initElasticPlan3D (size_t m1,
				                   size_t m2,
				                   size_t m3,
				                   double alpha,
				                   double mu,
				                   double lambda,
				                   double h1,
				                   double h2,
				                   double h3,
				                   boundary_t bc,
				                   elasticPlan * pPlan)
{

	elasticPlan     EP;
	EP = (elasticPlan) malloc (sizeof (elasticPlan_t));

	initElasticMatrix3D (m1, m2, m3, mu, lambda, h1, h2, h3, bc, &(EP->M3D));

	EP->in = (fftw_complex *) fftw_malloc (sizeof (fftw_complex) * m1 * m2 * m3);
	EP->out = (fftw_complex *) fftw_malloc (sizeof (fftw_complex) * m1 * m2 * m3);
	EP->pForw = fftw_plan_dft_3d (m1, m2, m3, EP->in, EP->out, FFTW_FORWARD, FFTW_MEASURE);
	EP->pBackw = fftw_plan_dft_3d (m1, m2, m3, EP->in, EP->out, FFTW_BACKWARD, FFTW_MEASURE);

	EP->alpha = alpha;
	EP->mu = mu;
	EP->lambda = lambda;
	EP->dim = 3;
	EP->m1 = m1;
	EP->m2 = m2;
	EP->m3 = m3;
	EP->h1 = h1;
	EP->h2 = h2;
	EP->h3 = h3;
	EP->bc = bc;

	*(pPlan) = EP;
}

/* --------------------------------------------------------------------- */
void            solveElastic3D (elasticPlan EP,
				                double ***in,
				                double ***out)
{
	int             k1;
	//, k2, k3;
	int             m1 = EP->m1;
	int             m2 = EP->m2;
	int             m3 = EP->m3;
	int             m123 = m1 * m2 * m3;
	double         *in1 = in[0][0];
	double         *in2 = in[1][0];
	double         *in3 = in[1][0];
	double          alpha = EP->alpha;
	array4D         invD = EP->M3D->data;
	fftw_complex   *fftIn1, *fftIn2, *fftIn3;
	double          maxX1, maxX2, maxX3;
	maxX1 = maxX2 = maxX3 = 0;

	/* allocate space for complex fft */
	fftIn1 = (fftw_complex *) fftw_malloc (sizeof (fftw_complex) * m123);
	fftIn2 = (fftw_complex *) fftw_malloc (sizeof (fftw_complex) * m123);
	fftIn3 = (fftw_complex *) fftw_malloc (sizeof (fftw_complex) * m123);

	/** fftIn1=fft3(in1) */

	/*
	 * hier werden die Daten von der uebergeordneten Struktur in den
	 * FFTW-Transformations-Kasten kopiert
	 */
	for (k1 = 0; k1 < m123; k1++) {
		EP->in[k1][0] = (-1) * in1[k1];	 /* Realteil */
		EP->in[k1][1] = 0;		 /* Imaginaerteil */
	}
	fftw_execute (EP->pForw);		 /* fuehre FFT aus */

	/*
	 * nun kopiere die Daten von der FFTW-Box in die eigentliche Datenstruktur
	 */
	for (k1 = 0; k1 < m123; k1++) {
		fftIn1[k1][0] = EP->out[k1][0];	 /* real */
		fftIn1[k1][1] = EP->out[k1][1];	 /* imag */
	}

	/* fftIn2=fft3(in2) */
	for (k1 = 0; k1 < m123; k1++) {
		EP->in[k1][0] = (-1) * in2[k1];	 /* real */
		EP->in[k1][1] = 0;		 /* imag */
	}
	fftw_execute (EP->pForw);		 /* ausfuehren FFT */
	for (k1 = 0; k1 < m123; k1++) {
		fftIn2[k1][0] = EP->out[k1][0];	 /* real */
		fftIn2[k1][1] = EP->out[k1][1];	 /* imag */
	}


	/* und fuer in3 ... */
	for (k1 = 0; k1 < m123; k1++) {
		EP->in[k1][0] = (-1) * in3[k1];	 /* real */
		EP->in[k1][1] = 0;		 /* imag */
	}
	fftw_execute (EP->pForw);		 /* ausfuehren FFT */
	for (k1 = 0; k1 < m123; k1++) {
		fftIn2[k1][0] = EP->out[k1][0];	 /* real */
		fftIn2[k1][1] = EP->out[k1][1];	 /* imag */
	}

	/* calculate du1 */
	/* du1 = invD11.*fft3(in1) + invD12.*fft3(in2) + invD13.*fft3(in3) */
	for (k1 = 0; k1 < m123; k1++) {
		EP->in[k1][0] = invD[0][0][0][k1] * fftIn1[k1][0] + invD[1][0][0][k1] * fftIn2[k1][0] + invD[2][0][0][k1] * fftIn3[k1][0];	/* real */
		EP->in[k1][1] = invD[0][0][0][k1] * fftIn1[k1][1] + invD[1][0][0][k1] * fftIn2[k1][1] + invD[2][0][0][k1] * fftIn3[k1][1];	/* imag */
	}
	fftw_execute (EP->pBackw);
	for (k1 = 0; k1 < m123; k1++) {
		out[0][0][k1] = EP->out[k1][0];
		maxX1 = maxAbs (maxX1, out[0][0][k1]);
	}

	/* calculate du2 */
	/* du2 = invD12.*fft3(in1) + invD22.*fft3(in2) + invD23.*fft3(in3) */
	for (k1 = 0; k1 < m123; k1++) {
		EP->in[k1][0] = invD[1][0][0][k1] * fftIn1[k1][0] + invD[3][0][0][k1] * fftIn2[k1][0] + invD[4][0][0][k1] * fftIn3[k1][0];	/* real */
		EP->in[k1][1] = invD[1][0][0][k1] * fftIn1[k1][1] + invD[3][0][0][k1] * fftIn2[k1][1] + invD[4][0][0][k1] * fftIn3[k1][1];	/* imag */
	}
	fftw_execute (EP->pBackw);
	for (k1 = 0; k1 < m123; k1++) {
		out[1][0][k1] = EP->out[k1][0];
		maxX2 = maxAbs (maxX2, out[1][0][k1]);
	}

	/* calculate du3 */
	/* du3 = invD13.*fft3(in1) + invD23.*fft3(in2) + invD33.*fft3(in3) */
	for (k1 = 0; k1 < m123; k1++) {
		EP->in[k1][0] = invD[2][0][0][k1] * fftIn1[k1][0] + invD[4][0][0][k1] * fftIn2[k1][0] + invD[5][0][0][k1] * fftIn3[k1][0];	/* real */
		EP->in[k1][1] = invD[2][0][0][k1] * fftIn1[k1][1] + invD[4][0][0][k1] * fftIn2[k1][1] + invD[5][0][0][k1] * fftIn3[k1][1];	/* imag */
	}
	fftw_execute (EP->pBackw);
	for (k1 = 0; k1 < m123; k1++) {
		out[2][0][k1] = EP->out[k1][0];
		maxX3 = maxAbs (maxX3, out[2][0][k1]);
	}

	/* free memory used by fft */
	fftw_free (fftIn1);
	fftw_free (fftIn2);
	fftw_free (fftIn3);

	printf ("\tmax(abs(dU1)) = %f, max(abs(dU2)) = %f\n, max(abs(dU2)) = %f\n", maxX1, maxX2, maxX3);
}

/*==============================================
/* Ende der 3d-Fall
/*==============================================

/* --------------------------------------------------------------------- */
void            deleteElasticMatrix2D (elasticMatrix2D A)
{
	if (A != NULL) {
		deleteArray3D (A->data);
		free (A);
	}
}

/* --------------------------------------------------------------------- */
void            deleteElasticPlan2D (elasticPlan EP)
{
	if (EP != NULL) {
		deleteElasticMatrix2D (EP->M2D);
		fftw_destroy_plan (EP->pForw);
		fftw_destroy_plan (EP->pBackw);
		fftw_free (EP->in);
		fftw_free (EP->out);
		free (EP);
	}
}

/* --------------------------------------------------------------------- */
void            deleteElasticMatrix3D (elasticMatrix3D A)
{
	if (A != NULL) {
		deleteArray4D (A->data);
		free (A);
	}
}

/* --------------------------------------------------------------------- */
void            deleteElasticPlan3D (elasticPlan EP)
{
	if (EP != NULL) {
		deleteElasticMatrix3D (EP->M3D);
		fftw_destroy_plan (EP->pForw);
		fftw_destroy_plan (EP->pBackw);
		fftw_free (EP->in);
		fftw_free (EP->out);
		free (EP);
	}
}
