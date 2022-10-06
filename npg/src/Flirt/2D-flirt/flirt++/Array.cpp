#include <Array.h>
#include <iostream>

namespace flirt
{

	// -------------------------------------------
	// Array
	// -------------------------------------------
	Array :: Array(int m1, int m2) {
		dims = NULL;
		data = NULL;
		init(m1, m2);
	};

	void Array::init(int m1, int m2) {
		ndims = 2;
		if (dims)
			delete dims;
		dims = new int[2];
		dims[0] = m1;
		dims[1] = m2;
		nelem = m1*m2;
		if (data)
			delete[] data;
		data = new double[nelem];
		memset(data, 0, sizeof(double)*nelem);
	};

	void Array::init(int m1, int m2, int m3) {
		ndims = 3;
		if (dims)
			delete dims;
		dims = new int[3];
		dims[0] = m1;
		dims[1] = m2;
		dims[2] = m3;
		nelem = m1*m2*m3;
		if (data)
			delete[] data;
		data = new double[nelem];
		memset(data, 0, sizeof(double)*nelem);
	};

	void Array::init(const int* _dims, int _ndims) {
		ndims = _ndims;
		dims  = new int[_ndims];
		nelem = 1;
		for (int i=0; i < ndims; ++i) {
			dims[i] = _dims[i];
			nelem *= dims[i];
		};
		data = new double[nelem];
		memset(data, 0, sizeof(double)*nelem);
	};

	Array:: Array(int m1, int m2, int m3) {
		dims = NULL;
		data = NULL;
		ndims = 3;
		init(m1, m2, m3);
	};

	Array:: Array(int m1, int m2, int m3, int m4) {
		ndims = 4;
		dims = new int[4];
		dims[0] = m1;
		dims[1] = m2;
		dims[2] = m3;
		dims[3] = m4;
		nelem = m1*m2*m3*m4;
		data = new double[nelem];
		memset(data, 0, sizeof(double)*nelem);
	};

	Array :: Array(const int* _dims, int _ndims) {
		ndims = _ndims;
		dims  = new int[_ndims];
		nelem = 1;
		for (int i=0; i < ndims; ++i) {
			dims[i] = _dims[i];
			nelem *= dims[i];
		};
		data = new double[nelem];
		memset(data, 0, sizeof(double)*nelem);
	};

	Array :: Array(const Array& A) {
		ndims = A.ndims;
		dims = new int[ndims];
		int i = 0;
		for(i=0; i < ndims; ++i ) {
			dims[i] = A.dims[i];
		};
		nelem = A.nelem;
		data = new double[nelem];
		for(i=0; i < nelem; ++i ) {
			data[i] = A.data[i];
		};
	};

	bool haveSameSize(const Array& A1, const Array& A2) {
		if (A1.getNumberOfDimensions() != A2.getNumberOfDimensions())
			return 0;

		for (int i = 0; i < A1.getNumberOfDimensions(); i++) {
			if (A1.getDimensions()[i] != A2.getDimensions()[i])
				return 0;
		};
		return 1;
	};

	void Array :: printToFile(FILE* file) const {};

	void Array :: print() const {printToFile(stdout);};

	void Array :: printDimensions() const
	{
		printf("%s %d\n","printDimensions of Array", ndims);
		for (int i=0; i<ndims; ++i)
			printf("%d \n", dims[i]);
		printf("\n%d elements\n", nelem);
	};

	// -------------------------------------------
	// Array 2D
	// -------------------------------------------
	Array2D :: Array2D()
		: A(NULL) {
	};

	Array2D :: Array2D(int m1, int m2)
		: Array( m1, m2) {
		A = createTreeStructure2D( m1, m2, data);
	};

	void Array2D::init(int m1, int m2) {
		Array::init(m1, m2);
		A = createTreeStructure2D( m1, m2, data);
	};

	Array2D :: Array2D(const int* _dims)
		: Array( _dims[0], _dims[1]) {
		A = createTreeStructure2D( _dims[0], _dims[1], data);
	};

	Array2D :: Array2D(const Array2D& A2)
		: Array(A2) {
		A = createTreeStructure2D( dims[0], dims[1], data);
	};

	void Array2D :: printToFile(FILE* file) const
	{
		printf("%s (%d %d)\n","Array2D::printToFile", dims[0], dims[1]);
		printToFileArray2D (A, dims[0], dims[1], file);
	};

	// -------------------------------------------
	// Array 3D
	// -------------------------------------------
	Array3D :: Array3D(int m1, int m2, int m3)
		: Array(m1, m2, m3) {
		A = createTreeStructure3D(m1, m2, m3, data);
	};

	Array3D :: Array3D(const Array3D& A3)
		: Array(A3) {
		A = createTreeStructure3D(dims[0], dims[1], dims[2], data);
	};

	void Array3D::init(int m1, int m2, int m3) {
		Array::init(m1, m2, m3);
		A = createTreeStructure3D( m1, m2, m3, data);
	};

	void Array3D :: printToFile(FILE* file) const
	{
		printf("%s (%d %d %d)\n","Array3D::printToFile", dims[0], dims[1], dims[2]);
		fprintf(file,"%d %d %d\n",dims[0], dims[1], dims[2]);
		for (int k1 = 0; k1 < dims[0]; k1++) {
			fprintf(file,"A[%d][][] = \n",k1);
			printToFileArray2D(A[k1],dims[1],dims[2],file);
		};
	};

	// -------------------------------------------
	// Array 4D
	// -------------------------------------------
	Array4D :: Array4D(int m1, int m2, int m3, int m4)
		: Array(m1, m2, m3, m4) {
		A = createTreeStructure4D(m1, m2, m3, m4, data);
	};

	Array4D :: Array4D(const Array4D& A4)
		: Array(A4) {
		A = createTreeStructure4D(dims[0], dims[1], dims[2], dims[3], data);
	};

	// Printing the content of an Array4D A into a given file.

	void Array4D :: printToFile(FILE* file) const
	{
		printf("Array4D::printToFile not implemented yet\n");
	};

}	// namespace
