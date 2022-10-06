//	File:	tools.h
//	Some very basic tools...

#ifndef _TOOLS_H_
#define _TOOLS_H_

#ifndef min
#define min(a,b) ((a)<(b) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) ((a)<(b) ? (b) : (a))
#endif

// C[i] = A[i] + B[i]
void add(const double* A, const double* B, double* C, size_t len);

// C[i] = A[i] - B[i]
void sub(const double* A, const double* B, double* C, size_t len);

// C[i] = alpha * B[i]
void mult(const double alpha, const double* B, double* C, size_t len);

// C[i] = A[i] * B[i]
void dot(const double* A, const double* B, double* C, size_t len);

// iprod(A,B) = sum_i A[i] * B[i]
double iprod(const double* A, const double* B, size_t len);

// A[i] = val
double setValue(double* A, size_t len, double val);

// norm2 = sqrt(iprod(A,A))
double norm2(const double* A, size_t len);

// normInf = max_i abs(A[i]
double normInf(const double* A, size_t len);

// dist2 = sum_i (A[i] - B[i])^2*/
double dist2(const double* A, const double* B, size_t len);

// distInf = max_i (abs(A[i] - B[i])
double distInf(const double* A, const double* B, size_t len);

#endif
