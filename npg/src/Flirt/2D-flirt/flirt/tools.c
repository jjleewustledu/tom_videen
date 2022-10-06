#include <stdlib.h>
#include <math.h>
#include "tools.h"

/* C[i] = A[i] + B[i]  */
void            add (const double *A, const double *B, double *C, size_t len)
{
	unsigned int    i;
	for (i = 0; i < len; ++i)
		C[i] = A[i] + B[i];

}

/* C[i] = A[i] - B[i]  */
void            sub (const double *A, const double *B, double *C, size_t len)
{
	unsigned int    i;
	for (i = 0; i < len; ++i)
		C[i] = A[i] - B[i];

}


/* C[i] = alpha * B[i]  */
void            mult (const double alpha, const double *B, double *C, size_t len)
{
	unsigned int    i;
	for (i = 0; i < len; ++i)
		C[i] = alpha * B[i];
}


/* C[i] = A[i] * B[i]  */
void            dot (const double *A, const double *B, double *C, size_t len)
{
	unsigned int    i;
	for (i = 0; i < len; ++i)
		C[i] = A[i] * B[i];
}


/* iprod(A,B) = sum_i A[i] * B[i]  */
double          iprod (const double *A, const double *B, size_t len)
{
	unsigned int    i;
	double          p = 0;
	for (i = 0; i < len; ++i)
		p += A[i] * B[i];
	return p;
}

/** \brief A[i] = val */
void            setValue (double *A, size_t len, double val)
{
	unsigned int    i;
	for (i = 0; i < len; ++i)
		A[i] = val;
}


/* norm2 = sqrt(iprod(A,A)) */
double          norm2 (const double *A, size_t len)
{
	return sqrt (iprod (A, A, len));
}


/* normInf = max_i abs(A[i] */
double          normInf (const double *A, size_t len)
{
	unsigned int    i;
	double          norm = 0.0;
	for (i = 0; i < len; ++i)
		norm = max (fabs (A[i]), norm);

	return norm;
}

/** \brief dist2 = sum_i (A[i] - B[i])^2*/
double          dist2 (const double *A, const double *B, size_t len)
{
	unsigned int    i;
	double          tmp;
	double          dist = 0.0;
	for (i = 0; i < len; i++) {
		tmp = A[i] - B[i];
		dist += tmp * tmp;
	}
	return dist;
}

/** \brief distInf = max_i (abs(A[i] - B[i]) */
double          distInf (const double *A, const double *B, size_t len)
{
	unsigned int    i;
	double          dist = 0.0;
	for (i = 0; i < len; i++)
		dist = min (fabs (A[i] - B[i]), dist);
	return dist;
}
