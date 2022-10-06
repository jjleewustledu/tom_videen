#include <flirt++.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

using namespace flirt;

typedef enum
{
	ELASTIC_FFT
}               smoother_t;

smoother_t      getSolverType (const char *str)
{
	return ELASTIC_FFT;
}


int             main (int argc, char *argv[])
{
	fprintf (stdout, "\n>>>   3D Elastic FFT (c) SAFIR, Luebeck, Germany, 2005-6   <<<\n\n");

	int             m1, m2, m3;
	double          h1 = 1.0, h2 = 1.0, h3 = 1.0;
	char           *fileNameT = (char *) malloc (255 * sizeof (char));
	char           *fileNameR = (char *) malloc (255 * sizeof (char));
	char           *fileNameOut = (char *) malloc (255 * sizeof (char));
	char           *fileNameU = (char *) malloc (255 * sizeof (char));
	char           *smootherName = (char *) malloc (255 * sizeof (char));
	int             maxIter = 10;
	double          alpha = 1.0;
	double          tau = 1.0;
	double          mu = 0.0;
	double          lambda = 0.0;
	int             iter = 0;
	time_t          ts1, ts2;
	double          dt;
	char           *pts;
	FILE           *ptr;

	if (argc > 1)
		getMainParameters3D (argv[1], &m1, &m2, &m3, &h1, &h2, &h3,
			fileNameT, fileNameR, fileNameOut, fileNameU, smootherName,
			&maxIter, &alpha, &tau, &mu, &lambda);
	else {
		fprintf (stderr,"ERROR [main]: no parameter file name given\n\n");
		return 0;
	}

	Image3D                 R (m1, m2, m3, h1, h2, h3);
	TransformableImage3D    T (m1, m2, m3, h1, h2, h3);

	R.readFromFile (fileNameR);
	T.readFromFile (fileNameT);

	Displacement3D          u (R.getDimensions (), h1, h2, h3);
	SumOfSquaredDifferences distancemeasure (R, T);
	ElasticSmoother         smoother;
	FPSolverElasticFFT3D    fpsolver (u, alpha, mu, lambda);
	FPOptimizer             opti (distancemeasure, smoother, fpsolver, alpha);

	// TMOptimizer			opti(distancemeasure,smoother,*solver);
	// opti.setParams(alpha,tau);
	// UpdateRule			ur(distancemeasure,smoother,opti);

	LineSearch              ur (distancemeasure, smoother, opti, T, u);
	StopRule                sr (distancemeasure, smoother, maxIter);

	T.transform (u);							  // the objective function: D[R,T(u)] + alpha*S[u]
	distancemeasure.eval ();
	distancemeasure.evalGradient ();
	smoother.eval (u);
	smoother.evalGradient (u);

	fprintf (stdout, "  n   dt(sec) |         D[u] \n");
	fprintf (stdout, "--------------+--------------------\n");
	fprintf (stdout, "%3d %8.1lf  | %15.5e\n", iter, 0, distancemeasure.getValue ());

	do {
		iter++;
		time(&ts1);								  // get current time
		opti.computeDisplacementUpdate (u);
		ur.updateDisplacement (u);

		T.transform (u);						  // the objective function: D[R,T(u)] + alpha*S[u]
		distancemeasure.eval ();
		distancemeasure.evalGradient ();
		smoother.eval (u);
		smoother.evalGradient (u);

		time(&ts2);								  // get current time (for loop timing)
		dt = difftime(ts2, ts1);

		// definierter Zustand: Alle Groessen uptodate ( T(u), D(u), S(u), gradD(u), gradS(u))

		fprintf (stdout, "%3d %8.1lf  | %15.5e      ", iter, dt, distancemeasure.getValue ());

		T.writeTransformedImageToFile (fileNameOut);

	} while (!sr.stopIteration (u, iter));

	if ((ptr = fopen (fileNameU, "wb")) != NULL) {
		fprintf (stdout, "Saving 3D transform to %s\n", fileNameU);
		u.writeTransform (ptr);
	} else {
		fprintf (stderr, "Error opening %s\n", fileNameU);
	};
}
