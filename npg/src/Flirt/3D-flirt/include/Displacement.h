//	Authors:	Stefan Heldmann, Nils Papenberg
//	Date:		28/04/2005

#ifndef _CLASS_DISPLACEMENT_H_
#define _CLASS_DISPLACEMENT_H_

#include "Array.h"

namespace       flirt
{

	class           Displacement;
	void            swapDisplacements (Displacement & U1, Displacement & U2);

	// Abstract Class for Displacements u = (u1, u2, ..., ud)
	// Author Nils Papenberg, Stefan Heldmann

	class           Displacement:public virtual Array
	{
		public:
			Displacement ();
			Displacement (const double *);
			Displacement (const Displacement &);
			virtual ~ Displacement ();

			// returns Voxelsize
			virtual const double *getVoxelSize () const;

			// friend method to swap displacement U1 with displacement U2
			friend void     swapDisplacements (Displacement & U1, Displacement & U2);

		protected:
			double         *h;
	};

	// Class for 2D Displacements U = (U1, U2)

	class           Displacement2D:public Displacement, public Array3D
	{

		public:

			Displacement2D ();
			Displacement2D (int, int, double, double);
			Displacement2D (const int *_dims, double h1, double h2);
			Displacement2D (const int *, const double *);

			// Copy constructor
			Displacement2D (const Displacement2D &);

			~Displacement2D ();

	};

	// Class for 3D Displacements U = (U1, U2, U3)

	class           Displacement3D:public Displacement, public Array4D
	{

		public:

			Displacement3D ();
			Displacement3D (int, int, int, double, double, double);
			Displacement3D (const int *_dims, double h1, double h2, double h3);
			Displacement3D (const int *, const double *);

			// Copy constructor
			Displacement3D (const Displacement3D &);

			~Displacement3D ();

	};

}
#endif
