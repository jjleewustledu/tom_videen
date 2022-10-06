#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <stdlib.h>
#include <stdio.h>

/** \file arrays.h
    \brief Routines for allocating and deallocation multi dimensional arrays

    Allows for random access of multidimensional array that are stored linear.
    Example: Let A a m-by-n array that is stored column-wise.
    Then the element A(i,j) is accessed by A[i+m*j]. For faster access to avoid index
    computations the methods below create indices, such that

    A(i,j) = A[i+m*j] = A[j][i].

    It is important that in the dimensions of an Array array must be passed in reverse order to
    the allocation methods!

    \author Stefan Heldmann
    \date 19/04/2005
*/


/** \brief Data type for arrays */
typedef double array_t;

/** \brief 1D array; A(i) = A[i] */
typedef array_t* array1D;

/** \brief 2D array; A(i,j) = A[j][i]*/
typedef array1D* array2D;

/** \brief 3D array; A(i,j,k) = A[k][j][i] */
typedef array2D* array3D;

/** \brief 4D array; A(i,j,k,l) = A[l][k][j][i] */
typedef array3D* array4D;

#
/** \brief Create a tree structure of a 2d array.
 */
array2D createTreeStructure2D(size_t m1, size_t m2, double* data);

/** \brief Frees the tree structure of a 2d array */
void    deleteTreeStructure2D(array2D A);



/** \brief Create a tree structure of a 3d array.
 */
array3D createTreeStructure3D(size_t m1, size_t m2, size_t m3, double* data);

/** \brief Frees the tree structure of a 3d array */
void    deleteTreeStructure3D(array3D A);

/** \brief Create a tree structure of a 4d array.
 */
array4D createTreeStructure4D(size_t m1, size_t m2, size_t m3, size_t m4, double* data);

/** \brief Frees the tree structure of a 4d array */
void    deleteArray4D(array4D A);

#

/** \brief Create a 2d array.
    For an m-by-n array call  \p createArray2D(n,m) such that
    \note A(i,j) = A[i+m*j] = A[j][i]
 */
array2D createArray2D(size_t m1, size_t m2);

/** \brief Frees a 2d array */
void    deleteArray2D(array2D A);



/** \brief Create a 3d array.
    For an m-by-n-by-p array call  \p createArray3D(p,n,m) such that
    \note A(i,j,k) = A[i+m*(j+n*k] = A[k][j][i]
 */
array3D createArray3D(size_t m1, size_t m2, size_t m3);

/** \brief Frees a 3d array */
void    deleteArray3D(array3D A);

/** \brief Create an 4d array.
    For an m-by-n-by-p-by-q array call  \p createArray4D(q,p,n,m) such that
    \note A(i,j,k,l) = A[i+m*(j+n*(k+p*l)] = A[l][k][j][i]
 */
array4D createArray4D(size_t m1, size_t m2, size_t m3, size_t m4);

/** \brief Frees a 4d array */
void    deleteArray4D(array4D A);

/** brief Prints array2D A of size m1 x m2 to file */
void printToFileArray2D(array2D A, int m1, int m2, FILE* file);




#endif
