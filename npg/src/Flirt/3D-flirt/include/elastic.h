//	File elastic.h
//	Low-level routiones for elastic registration.
//	Authors:	Stefan Wirtz and Konstantin Ens
//	Date:		12/12/2005

#ifndef _ELASTIC_H_
#define _ELASTIC_H_

#include "defs.h"
#include "flirt.h"
#include <fftw3.h>
#include <math.h>

//	General structure for operator matrix.
//	Holds variables inverse matrix.
//	Author Stefan Wirtz
//	Date 26/07/2005

typedef struct
{
	array3D data;
	size_t len;
} elasticMatrix_t;

typedef struct
{
	array4D data;
	size_t len;
} elasticMatrix3D_t;

typedef elasticMatrix_t* elasticMatrix;
typedef elasticMatrix3D_t* elasticMatrix3D;

//	General plan for elastic solver.
//	Holds workspace and variables for 2 and 3 dimensional transforms.
//	Author Stefan Wirtz
//	Date 26/07/2005

typedef struct
{
	elasticMatrix M;							  // Matrix with pseudoinverse of elastic operator matrix
	fftw_plan pForw;							  // FFT forward plan
	fftw_plan pBackw;							  // FFT backward plan
	fftw_complex* in;							  // FFT memory space for input data
	fftw_complex* out;							  // FFT memory space for input data
	double alpha;								  // Smoothness Parameter
	double mu;									  // First Lame constant
	double lambda;								  // Second Lame constant
	int dim;									  // Dimension of Registration Problem
	double h1, h2, h3;							  // Voxelsize
	int m1, m2, m3;								  // Number of Voxels
	boundary_t bc;								  // Boundary Condition
} elasticPlan_t;

typedef struct
{
	elasticMatrix3D M;							  // Matrix with pseudoinverse of elastic operator matrix
	fftw_plan pForw;							  // FFT forward plan
	fftw_plan pBackw;							  // FFT backward plan
	fftw_complex* in;							  // FFT memory space for input data
	fftw_complex* out;							  // FFT memory space for input data
	double alpha;								  // Smoothness Parameter
	double mu;									  // First Lame constant
	double lambda;								  // Second Lame constant
	int dim;									  // Dimension of Registration Problem
	double h1, h2, h3;							  // Voxelsize
	int m1, m2, m3;								  // Number of Voxels
	boundary_t bc;								  // Boundary Condition
} elasticPlan3D_t;

//	General plan for elastic solver.
//	Holds workspace and variables for 2 and 3 dimensional transforms.
//	Author Stefan Wirtz
//	Date 26/07/2005

typedef elasticPlan_t* elasticPlan;
typedef elasticPlan3D_t* elasticPlan3D;

//	For developing purpose: Routine for printing the content of an elastic plan to stdout.
//	Author Stefan Wirtz
//	Date 26/07/2005

void printElasticPlan(const elasticPlan M);

//	Creates the system matrix for elastic solver in 2D.
//	Authors Stefan Wirtz and Konstantin Ens
//	Date 12/12/2005
//	@param m1 number of pixel in x1-direction
//	@param m2 number of pixel in x2-direction
//	@param mu First Lame constant
//	@param lambda Second Lame constant
//	@param h1 Voxelsize in x1-Dimension
//	@param h2 Voxelsize in x2-Dimension
//	@param bc Boundary Condtion
//	@param pMatrix Pointer to elastic system matrix.

void initElasticMatrix2D(size_t m1,
	size_t m2,
	double mu,
	double lambda,
	double h1,
	double h2,
	boundary_t bc,
	elasticMatrix* pMatrix);

//	Creates and initializes the elasticMatrix for 2D transforms.
//	Author Stefan Wirtz and Konstantin Ens
//	Date 12/12/2005
//	@param m1 number of Voxels in x1-Dimension
//	@param m2 number of Voxels in x2-Dimension
//	@param alpha smoothness factor alpha
//	@param mu First Lame constant
//	@param lambda Second Lame constant
//	@param h1 Voxelsize in x1-Dimension
//	@param h2 Voxelsize in x2-Dimension
//	@param bc Boundary Condition
//	@param M out: elastic Plan

void initElasticPlan2D(size_t m1,
	size_t m2,
	double alpha,
	double mu,
	double lambda,
	double h1,
	double h2,
	boundary_t bc,
	elasticPlan *M);

//	Solve LS of Elastic Registration 2D where 'in' is the rhs of the system and 'out' means u^{k+1}.
//	@param EP  elastic Plan
//	@param in  right hand side of the system
//	@param out output

void solveElastic2D(elasticPlan EP,
	double*** in,
	double*** out);

//	==============================================
//		3d-Fall
//	==============================================
//	Creates the system matrix for elastic solver in 3D.
//	Authors Stefan Wirtz and Konstantin Ens
//	Date 12/12/2005
//	@param m1 number of pixel in x1-direction
//	@param m2 number of pixel in x2-direction
//	@param m3 number of pixel in x3-direction
//	@param mu First Lame constant
//	@param lambda Second Lame constant
//	@param h1 Voxelsize in x1-Dimension
//	@param h2 Voxelsize in x2-Dimension
//	@param h3 Voxelsize in x3-Dimension
//	@param bc Boundary Condtion
//	@param pMatrix Pointer to elastic system matrix.

void initElasticMatrix3D(size_t m1,
	size_t m2,
	size_t m3,
	double mu,
	double lambda,
	double h1,
	double h2,
	double h3,
	boundary_t bc,
	elasticMatrix3D* pMatrix);

//	Creates and initializes the elasticMatrix for 3D transforms.
//	Authors Stefan Wirtz and Konstantin Ens
//	Date 12/12/2005
//	@param m1 number of Voxels in x1-Dimension
//	@param m2 number of Voxels in x2-Dimension
//	@param m3 number of pixel in x3-direction
//	@param alpha smoothness factor alpha
//	@param mu First Lame constant
//	@param lambda Second Lame constant
//	@param h1 Voxelsize in x1-Dimension
//	@param h2 Voxelsize in x2-Dimension
//	@param h3 Voxelsize in x3-Dimension
//	@param bc Boundary Condition
//	@param M out: elastic Plan

void initElasticPlan3D(size_t m1,
	size_t m2,
	size_t m3,
	double alpha,
	double mu,
	double lambda,
	double h1,
	double h2,
	double h3,
	boundary_t bc,
	elasticPlan3D *M);

//	Solve LS of Elastic Registration 3D where 'in' is the rhs of the system and 'out' means u^{k+1}.
//	Authors Stefan Wirtz and Konstantin Ens
//	Date 12/12/2005
//	@param EP  elastic Plan
//	@param in  right hand side of the system
//	@param out output

void solveElastic3D(elasticPlan3D EP,
	double**** in,
	double**** out);

//	==============================================
//		Ende der 3d-Fall
//	==============================================

//	Frees the memory of elasticMatrix.
//	Author Stefan Wirtz
//	Date 26/07/2005

void deleteElasticMatrix(elasticMatrix A);
void deleteElasticMatrix3D(elasticMatrix3D A);

//	Frees the memory of elasticPlan M.
//	Author Stefan Wirtz
//	Date 26/07/2005

void deleteElasticPlan(elasticPlan M);
void deleteElasticPlan3D(elasticPlan3D M);

#endif
