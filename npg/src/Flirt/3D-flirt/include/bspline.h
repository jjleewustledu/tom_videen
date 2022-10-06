//	File bspline.h
//	Tools for bspline interpolation
//	Author Stefan Heldmann
//	Date 19/04/2005

#ifndef _BSPLINE_H_
#define _BSPLINE_H_

#include "defs.h"	// required for boundary_t

// Compute 1D bspline coefficient for the equispaced grid 0,1,..,N-1
void computeBSplineCoeff1D(double I[],int N,double C[]);

// Compute 2D bspline coefficient for the equispaced grid {0,1,..,m-1}x{0,1,..,n-1}
// It is required that I is stored in column-major order; I(i,j) = I[i+m*j]
void computeBSplineCoeff2D(double I[],int m ,int n,double C[]);

// Compute 2D bspline coefficient for the equispaced grid {0,1,..,m-1}x{0,1,..,n-1} in place.
// It is required that I is stored in column-major order; I(i,j) = I[i+m*j]
void convertToBSplineCoeff2D(double* C, int m1, int m2);

// Compute 2D aproximate bspline coefficient for the equispaced grid {0,1,..,m1-1}x{0,1,..,m2-1}.
// such that |I - B*C| + tau*|A*B*C| = min, whereby A the laplacian with boundary condition bc
// It is required that I is stored in column-major order; I(i,j) = I[i+m1*j]
void computeApproxBSplineCoeff2D(double* I,int m1, int m2, double tau, boundary_t bc, double* C);

// Compute 3D bspline coefficient for the equispaced grid {0,1,..,m1-1}x{0,1,..,m2-1}x{0,1,..,m3-1}.
// It is required that I is stored in column-major order; I(i,j,k) = I[i+m1*(j+m2*k)]
void computeBSplineCoeff3D(double I[], int m, int n, int p, double C[]);

// Compute 3D aproximate bspline coefficient in place.
// It is required that I is stored in column-major order; I(i,j,k) = I[i+m1*(j+m2*k)]
void convertToBSplineCoeff3D(double* C, int m1, int m2,int m3);

// Compute 3D aproximate bspline coefficient for the equispaced grid {0,1,..,m1-1}x{0,1,..,m2-1}x{0,1,..,m3-1}.
// such that |I - B*C| + tau*|A*B*C| = min, whereby A the laplacian with boundary condition bc
// It is required that I is stored in column-major order; I(i,j,k) = I[i+m1*(j+m2*k)]
void computeApproxBSplineCoeff3D(double* I, int m1, int m2, int m3, double tau, boundary_t bc, double* C);

// Evaluate 1D bspline coefficients
void evalBSpline1D(double C[], int m , double x[],  int N, double S[]);

// Evaluate 1D bspline coefficients and first derivatives
void evalBSplineAndGrad1D(double C[], int m , double x[], int N, double S[], double d_S[] );

// Evaluate 2D bspline coefficients
void evalBSpline2D(double C[], int m , int n, double x[], double y[], int N, double S[]);

// Evaluate 2D bspline coefficients and first derivatives
void evalBSplineAndGrad2D(double C[], int m , int n, double x[], double y[], int N, double S[], double d1_S[], double d2_S[] );

// Evaluate 3D bspline coefficients
void evalBSpline3D(double C[], int m , int n, int p, double x[], double y[], double z[], int N, double S[]);

// Evaluate 3D bspline coefficients and first derivatives
void evalBSplineAndGrad3D(double C[],
	int m ,
	int n,
	int p,
	double x[],
	double y[],
	double z[],
	int N,
	double S[],
	double d1_S[],
	double d2_S[],
	double d3_S[] );

#endif
