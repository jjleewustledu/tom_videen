//	File diffusive.h
//	Low-level routiones for diffusive registration
//	Author Nils Papenberg
//	Date 20/04/2005

#ifndef _DIFFUSIVE_H_
#define _DIFFUSIVE_H_

#include "defs.h"
#include "flirt.h"

//	General structure for tridiagonal matrix
//	Holds variables for tridiag Matrix

typedef struct
{
	double* diag;
	double* low;
	double* up;
	size_t len;
} triDiagMatrix_t;

typedef triDiagMatrix_t* triDiagMatrix;

//	General plan for diffusive solver with AOS.
//	Holds worspace and variables for 2 and 3 dimensional transforms

typedef struct
{
	triDiagMatrix M1;							  // Matrix in x1-dimension
	triDiagMatrix M2;							  // Matrix in x2-dimension
	triDiagMatrix M3;							  // Matrix in x3-dimension
	array2D tmp;								  // Auxilliary Variable
	double alpha;								  // Smoothness Parameter
	double tau;									  // timestep parameter
	int dim;									  // Dimension of Registration Problem
	double h1, h2, h3;							  // Voxelsize
	int m1, m2, m3;								  // Number of Voxels
	boundary_t bc;								  // Boundary Condition
} diffusivePlan_t;

//	General plan for diffusive solver.
//	Holds worspace and variables for 2 and 3 dimensional transforms

typedef diffusivePlan_t* diffusivePlan;

// sadhla ksf
void printDiffusivePlan(const diffusivePlan M);
void printTriDiagMatrix(const triDiagMatrix A);

//	Creates Tridiagonal Matrix for AOS in one direction I - alphatau A
//	@param m Lenth of main diagonal of Matrix M
//	@param alphatau The scale factor for Matrix A
//	@param bc Boundary Condtion
//	@param M Pointer of Output Matrrx

void initTriDiagMatrix(size_t m,
	double alphatau,
	boundary_t bc,
	triDiagMatrix *M);

//	Creates and initializes the diffuisveMatrix for 2d transforms
//	@param m1 number of Voxels in x1-Dimension
//	@param m2 number of Voxels in x2-Dimension
//	@param alpha smoothness factor alpha
//	@param tau timestep parameter
//	@param h1 Voxelsize in x1-Dimension
//	@param h2 Voxelsize in x2-Dimension
//	@param bc Boundary Condition
//	@param M out: diffusive Plan

void initDiffusivePlan2D(size_t m1,
	size_t m2,
	double alpha,
	double tau,
	double h1,
	double h2,
	boundary_t bc,
	diffusivePlan *M);

//	Solve the TridiagonalSystem Ax = b
//	@param A Tridiagonalmatrix A
//	@param b Pointer to Righthand side
//	@param inc Increment between b(i) and b(i+1) and x(i) and x(i+1)
//	@param x Pointer to output

void solveThomas(triDiagMatrix A,
	double* b,
	int inc,
	double* x);

//	Solve LS of Diffusive Registration 2Dwith AOS where b is the rhs of the system and x means u^{k+1}
//	@param A diffusive PLan
//	@param b right hand side of the system
//	@param x output

void solveDiffusive2D(diffusivePlan M,
	double** b,
	double** x);

// Compute Ax
// void evalDiffusive2D(diffusivePlan A,
//	     double** x,
//	     size_t m1,
//	     size_t m2,
//	     double** Ax);

//	Frees the memory of a triDiagMatrix

void deleteTriDiagMatrix(triDiagMatrix A);

//	Frees the memory of diffusivePlan M

void deleteDiffusivePlan(diffusivePlan M);

#endif
