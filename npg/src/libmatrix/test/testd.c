#include <stdio.h>
#include <nrecipes/kr/nr.h>
#include <nrecipes/kr/nrutil.h>
#include <petutil/matrixlib.h>

#define M 2
#define N 2

main ()
{
	double        **a, **b, **c, **d, **e;
	double        *f, *g;
	int             i, j, k;

	a = dmatrix (1, M, 1, N);
	b = dmatrix (1, M, 1, N);
	c = dmatrix (1, M, 1, N);
	d = dmatrix (1, M, 1, N);
	e = dmatrix (1, M, 1, N);
	f = dvector (1, N);
	g = dvector (1, N);

	a[1][1] = 1;
	a[1][2] = 3;
	a[2][1] = 0;
	a[2][2] = 2;
	dmatrix_print (a, M, N);

	b[1][1] = -5;
	b[1][2] = 4;
	b[2][1] = 2;
	b[2][2] = 2;
	dmatrix_print (b, M, N);

	dmatrix_add (a, b, c, M, N);
	dmatrix_print (c, M, N);

	dmatrix_mult (a, b, d, M, N, N);
	dmatrix_print (d, M, N);

	for (k = 0, i = 1; i <= M; i++)
		for (j = 1; j <= N; j++)
			e[i][j] = (double) k++;

	for (k = 0, i = 1; i <= M; i++) {
		for (j = 1; j <= N; j++)
			printf ("%10.4f  ", e[i][j]);
		printf ("\n");
	}
	printf ("\n");

	f[1] = 1;
	f[2] = 2;
	dmatrix_print (f, N, 1);

    dmatrix_mult (a, f, g, M, N, 1);
    dmatrix_print (g, N, 1);
}
