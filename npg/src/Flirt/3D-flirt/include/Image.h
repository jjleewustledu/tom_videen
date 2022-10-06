// Authors:	Nils Papenberg, Stefan Heldmann, Konstantin Ens
// 			Silke Bommersheim, Bernd Fischer

#ifndef _CLASS_IMAGE_H_
#define _CLASS_IMAGE_H_

#include <stdio.h>
#include <Array.h>
#include <Displacement.h>

namespace flirt
{

	class Image : public virtual Array
	{
		public:
			Image():h(NULL) {};

			// Copy constructor
			Image(const Image& );

			virtual ~Image() {
				delete[] h;
			};

			virtual const double* getVoxelSize() const;

		protected:
			double *h;
	};

	class TransformableImage : public virtual Image
	{
		public:
			// compute transformed image T(x + u(x))
			virtual void transformImage(const Displacement& u) = 0;

			// compute (grad T)(x + u(x))
			virtual void transformGradient(const Displacement& u) = 0;

			// compute T(x + u(x)) and (grad T)(x + u(x))
			virtual void transform(const Displacement &u) = 0;

			// get the transformed image  T(x + u(x))
			virtual const Array& getTransformedImage() const = 0;

			// get (grad T)(x + u(x))
			virtual const Array& getTransformedGradient() const = 0;
	};

	class Image3D : public virtual Image, public Array3D
	{
		public:
			Image3D():Array3D() {};

			Image3D( int, int, int, double, double, double);

			Image3D( int*, double*);

			Image3D ( char *, double, double, double);

			// Copy constructor
			Image3D( const Image3D&);

			virtual ~Image3D() {}

			void init(char *name, double h1, double h2, double h3);

			// write Image3D to File name
			void writeToFile(char *name) const;

			// read Image3D from File name
			void readFromFile(char *name);

			void writeToConsole() const;
	};

	class TransformableImage3D   : public  Image3D, public TransformableImage
	{
		public:
			TransformableImage3D();

			TransformableImage3D( int, int, int, double, double, double);

			// make TransformableImage3D from TIFF file
			TransformableImage3D( char *, double, double, double);

			// Copy constructor
			TransformableImage3D( const TransformableImage3D&);

			~TransformableImage3D() {};

			// Imagedata and also Imagedimensions are read from an Tiff file
			void init(char *name, double h1, double h2, double h3);

			// compute transformed image T(x + u(x))
			virtual void transformImage(const Displacement& u);

			// compute (grad T)(x + u(x))
			virtual void transformGradient(const Displacement& u);

			// compute T(x + u(x)) and (grad T)(x + u(x))
			virtual void transform(const Displacement& u);

			// get the transformed image  T(x + u(x))
			virtual const Array& getTransformedImage() const;

			// get (grad T)(x + u(x))
			virtual const Array& getTransformedGradient() const;

			// write transformed Image to file
			virtual void writeTransformedImageToFile(char* name);

			void writeTransformedImageToConsole() const;

		private:
			Array3D transformedImage;
			Array4D transformedGradient;

	};

}	// namespace
#endif
