#include "Displacement.h"
#include "Array.h"

namespace flirt
{

	Displacement::Displacement():h(NULL) {};

	Displacement :: Displacement(const double* _h) {
		h = new double[ndims];
		for (int k1 = 0; k1 < ndims; k1++) {
			h[k1] = _h[k1];
		}
	};

	Displacement :: ~Displacement() {
		if (h != NULL) delete[] h;
	};

	const double* Displacement::getVoxelSize() const
	{
		return h;
	};

	// ------------------------------------------------
	// Displacement2D
	// ------------------------------------------------
	Displacement2D :: Displacement2D() {
		h = NULL;
	};

	void Displacement2D::init(int m1, int m2, double h1, double h2) {
		Array::init(2, m1, m2);
		Array3D::init(2, m1, m2);
		if (h) {
			delete[] h;
		};
		h = new double[2];
		h[0] = h1;
		h[1] = h2;
	};

	void Displacement2D::reset() {
		double* data = getData();
		int size = getNumberOfElements();
		memset(data, 0, size*sizeof(double));
	};

	Displacement2D :: Displacement2D( int m1, int m2, double h1, double h2)
		: Array(2, m1, m2), Array3D( 2, m1, m2) {
		h = new double[2];
		h[0] = h1;
		h[1] = h2;
	};

	Displacement2D :: Displacement2D( const int* _dims, double h1, double h2)
		: Array(2, _dims[0], _dims[1]), Array3D( 2, _dims[0], _dims[1]) {
		h = new double[2];
		h[0] = h1;
		h[1] = h2;
	};

	Displacement2D :: Displacement2D(const int* _dims, const double* h)
		: Array(2, _dims[0], _dims[1]), Array3D( 2, _dims[0], _dims[1]), Displacement( h) {
	};

	Displacement2D :: Displacement2D( const Displacement2D& u)
		: Array( u), Array3D(u) {
		h = new double[2];
		h[0] = u.h[0];
		h[1] = u.h[1];
	};

	Displacement2D :: ~Displacement2D() {};

	// ------------------------------------------------
	// Displacement 3D
	// ------------------------------------------------
	Displacement3D :: Displacement3D() {
	};

	Displacement3D :: Displacement3D( int m1, int m2, int m3, double h1, double h2, double h3)
		: Array(3, m1, m2, m3), Array4D(3, m1, m2, m3) {
		h = new double[3];
		h[0] = h1;
		h[1] = h2;
		h[2] = h3;
	};

	Displacement3D :: Displacement3D( const int* _dims, double h1, double h2, double h3)
		: Array(3, _dims[0], _dims[1], _dims[2]), Array4D(3, _dims[0], _dims[1], _dims[2]) {
		h = new double[3];
		h[0] = h1;
		h[1] = h2;
		h[2] = h3;
	};

	Displacement3D :: ~Displacement3D() {
	};

}	//namespace flirt


//	Swap Displacement U1 with U2.
//	Just the data pointers are swap. The rest of the attributes are unchanged.

void flirt::swapDisplacements(Displacement &U1, Displacement &U2)
{
	double* tmp = U1.data;
	U1.data = U2.data;
	U2.data = tmp;

};
