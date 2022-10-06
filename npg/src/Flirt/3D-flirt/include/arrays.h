//	File arrays.h
//	Routines for allocating and deallocation multi dimensional arrays
//	Allows for random access of multidimensional array that are stored linear.
//	Example: Let A a m-by-n array that is stored column-wise.
//	Then the element A(i,j) is accessed by A[i+m*j]. For faster access to avoid index
//	computations the methods below create indices, such that
//
//		A(i,j) = A[i+m*j] = A[j][i].
//
//	It is important that in the dimensions of an Array array must be passed
//	in reverse order to the allocation methods!
//	Author Stefan Heldmann
//	Date 19/04/2005

#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <stdlib.h>
#include <stdio.h>

typedef double array_t;			  // Data type for arrays
typedef array_t* array1D;		  // 1D array; A(i) = A[i]
typedef array1D* array2D;		  // 2D array; A(i,j) = A[j][i]
typedef array2D* array3D;		  // 3D array; A(i,j,k) = A[k][j][i]
typedef array3D* array4D;		  // 4D array; A(i,j,k,l) = A[l][k][j][i]

// 2D arrays: Create/Free tree structure/array; Print to file
//	For an m-by-n array call createArray2D(n,m) such that
//		A(i,j) = A[i+m*j] = A[j][i]

array2D createTreeStructure2D(size_t m1, size_t m2, double* data);
void    deleteTreeStructure2D(array2D A);
array2D createArray2D(size_t m1, size_t m2);
void    deleteArray2D(array2D A);
void	printToFileArray2D(array2D A, int m1, int m2, FILE* file);

// 3D arrays: Create/Free tree structure
//	For an m-by-n-by-p array call  createArray3D(p,n,m) such that
//		A(i,j,k) = A[i+m*(j+n*k] = A[k][j][i]

array3D createTreeStructure3D(size_t m1, size_t m2, size_t m3, double* data);
void    deleteTreeStructure3D(array3D A);
array3D createArray3D(size_t m1, size_t m2, size_t m3);
void    deleteArray3D(array3D A);
void	printToFileArray3D(array3D A, int m1, int m2, int m3, FILE* file);

// 4D arrays: Create/Free tree structure
//	For an m-by-n-by-p-by-q array call  createArray4D(q,p,n,m) such that
//		A(i,j,k,l) = A[i+m*(j+n*(k+p*l)] = A[l][k][j][i]

array4D createTreeStructure4D(size_t m1, size_t m2, size_t m3, size_t m4, double* data);
void    deleteTreeStructure4D(array4D A);
array4D createArray4D(size_t m1, size_t m2, size_t m3, size_t m4);
void    deleteArray4D(array4D A);
void	printToFileArray4D(array4D A, int m1, int m2, int m3, int m4, FILE* file);
void	writeTransform4D(array4D A, int m1, int m2, int m3, int m4, FILE* file);
void	readTransform4D(array4D A, int m1, int m2, int m3, int m4, FILE* file);

#endif
