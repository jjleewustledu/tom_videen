//	File:		Array.h
//	Defines:	Array class
//	Authors:	Stefan Heldmann, Nils Papenberg
//	Date:		28/04/2005

#ifndef _CLASS_ARRAY_H_
#define _CLASS_ARRAY_H_

#include <stdio.h>
#include <flirt.h>

namespace flirt {

	class Array;
	bool haveSameSize(const Array& A1, const Array& A2);

	class Array {
	public:
	
		Array()
		  :	dims(NULL),
			ndims(0),
			nelem(0),
			data(NULL) {
		};

		Array(int, int);
	 	Array(int, int, int);
	 	Array(int, int, int, int);
	 	Array(const int*, int);
	
		// Copyconstructor
		Array(const Array&);
	
		virtual ~Array() {
	 		if (dims != NULL) {
				delete[] dims;
				dims = NULL;
				delete[] data;
	 		}
		};
	
		// Get data[i]
		inline double& operator()(int i) { return data[i]; };

		// Get const data[i]
		inline double operator()(int i) const { return data[i]; };

		// Get data[i]
		inline double& operator[](int i) { return data[i]; };
	
		// Get const data[i] 
		inline double operator[](int i) const { return data[i]; };
	
		// Returns Number of Dims: ndims
		inline int getNumberOfDimensions() const { return ndims; };
	
		// Returns const pointer of Dimensions
		inline const int* getDimensions() const { return dims; }
	
		// Returns pointer of data as an array
		inline double* getData() { return data; };
	
		// Returns pointer of data as double*
		inline const double* getData() const { return data; };
	
		// Returns Number of Elements
		inline int getNumberOfElements() const { return nelem; };

		virtual void printToFile(FILE* file) const;
		virtual void print() const;
		virtual void printDimensions() const;

	protected:
		int* dims;			// pointer to dimension
		int ndims;			// number of dimensions
		int nelem;			// number of elements 
		double* data;		// pointer to Array Dada
	
	};

// Class for 2D scalar data or 1D vector data

	class Array2D : public virtual Array {
	
	public:
	
		// Construct Array2D of size m1 x m2 
		Array2D();
		Array2D( int, int);
		Array2D( const int*);

		Array2D( const Array2D&);		

		virtual ~Array2D() {
			if (A != NULL) { 
				deleteTreeStructure2D(A);
				A = NULL;
			}
		};

		// Overloading []-operator -- A[i][j] returns Element at (i,j)
		inline virtual array1D operator[](int i) { return A[i]; };
	
		// Overloading []-operator for const Array2D -- A[i][j] returns Element at (i,j)
		inline virtual const array1D operator[](int i) const {return A[i]; };
	
		// Returns const pointer of Array A 
		inline virtual const array2D getArray() const {return A;};
	
		// Returns pointer of Array A 
		inline virtual array2D getArray() {return A;};
	
		// Prints Array2D to a given File
		virtual void printToFile(FILE* file) const;

	protected:
		array2D A;		// 2D array data

	};

// Class for 3D scalar data or 2D vector data

	class Array3D : public virtual Array {
	
	public:

		// Construct Array3D of size m1 x m2 x m3
		Array3D():A(NULL) {};
	
		Array3D(int, int, int);	

		// Copyconstructor 
		Array3D(const Array3D&);

		virtual Array3D::~Array3D() {
			if (A != NULL){
		 		deleteTreeStructure3D(A);
		 		A = NULL;
			}
		};
	
		// Overloading []-operator -- A[i][j][k] returns Element at (i,j,k)
		inline virtual array2D operator[](int i) { return A[i]; };
	
		// Overloading []-operator for const Array3D -- A[i][j][k] returns Element at (i,j,k)
		inline virtual const array2D operator[](int i) const {return A[i]; };

		// Returns const pointer of array A 
		inline virtual const array3D getArray() const {return A;};
	
		// Returns pointer of array A
		inline virtual array3D getArray() {return A;};

		// Prints Array3D to a given file
		virtual void printToFile(FILE* file) const;
	
	protected:
		array3D A;		// 3D array data

	};

// Class for 4D scalar data or 3D vector data

	class Array4D : public virtual Array {
	
	public:

		// Construct Array4D of size m1 x m2 x m3 x m4
		Array4D():A(NULL) {};
	
		Array4D(int, int, int, int);	

		// Copyconstructor
		Array4D(const Array4D&);

		virtual Array4D::~Array4D() {
			if (A != NULL){
				deleteTreeStructure4D(A);
				A = NULL;
			}
		};
	
		// Overloading []-operator -- A[i][j][k][l] returns Element at (i,j,k,l)
		inline virtual array3D operator[] (int i) { return A[i]; };
	
		// Overloading []-operator for const Array4D -- A[i][j][k][l] returns Element at (i,j,k,l)
		inline virtual const array3D operator[] (int i) const {return A[i]; };
	
		// Returns const pointer of array A 
		inline virtual const array4D getArray() const {return A;};
	
		// Returns pointer of array A
		inline virtual array4D getArray() {return A;};

		// Prints Array4D to a given file
		virtual void printToFile(FILE* file) const;

		// Write Array4D to a given file
		virtual void writeTransform(FILE* file) const;

		// Reads Array4D from a given file
		virtual void readTransform(FILE* file) const;

		// Prints Array3D to a given file 
		void writeToConsole() const;
	
	protected:
		array4D A;		// 4D array data
	};

} // namespace

#endif
