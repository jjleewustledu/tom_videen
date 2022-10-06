#include <stdio.h>
#include <nrecipes/kr/nr.h>
#include <nrecipes/kr/nrutil.h>
#include <petutil/matrixlib.h>

#define M 2
#define N 2

main ()
{
	float        **a, **b, **c, **d, **e;
	float		*f, *g;
	int             i, j, k;

	a = matrix (1, M, 1, N);
	b = matrix (1, M, 1, N);
	c = matrix (1, M, 1, N);
	d = matrix (1, M, 1, N);
	e = matrix (1, M, 1, N);
	f = vector (1, N);
	g = vector (1, N);

	a[1][1] = 1;
	a[1][2] = 3;
	a[2][1] = 0;
	a[2][2] = 2;
	matrix_print (a, M, N);

	b[1][1] = -5;
	b[1][2] = 4;
	b[2][1] = 2;
	b[2][2] = 2;
	matrix_print (b, M, N);

	matrix_add (a, b, c, M, N);
	matrix_print (c, M, N);

	matrix_mult (a, b, d, M, N, N);
	matrix_print (d, M, N);

	for (k = 0, i = 1; i <= M; i++)
		for (j = 1; j <= N; j++)
			e[i][j] = (float) k++;

	for (k = 0, i = 1; i <= M; i++) {
		for (j = 1; j <= N; j++)
			printf ("%10.4f  ", e[i][j]);
		printf ("\n");
	}
	printf ("\n");

    f[1] = 1;
    f[2] = 2;
    matrix_print (f, N, 1);

	matrix_mult (a, f, g, M, N, 1);
	matrix_print (g, N, 1);
}
