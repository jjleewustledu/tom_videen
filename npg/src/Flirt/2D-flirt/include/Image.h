#ifndef _CLASS_IMAGE_H_
#define _CLASS_IMAGE_H_

#include <stdio.h>
#include "Array.h"
#include "Displacement.h"

namespace flirt {

/**
 * \brief An image
 * \author Nils Papenberg, Stefan Heldmann
 */
class Image : public virtual Array {
public:
    Image(void) : h(NULL) {
      //printf("Constructor Image()\n\n");
    };

    /**
     * \brief Copyconstructor
     */
    Image(const Image& );

    virtual ~Image() {
        //printf("Destructor Image()\n\n");
        delete[] h;
    };


    /**
     * get Voxelsize
     */
    virtual const double* getVoxelSize() const;

protected:
    double *h;
};


/**
 * \brief TransformableImage
 * \author Stefan Heldmann, Nils Papenberg
 */

class TransformableImage : public virtual Image {
public:
    /**
     * compute transformed image T(x + u(x))
     */
    virtual void transformImage(const Displacement& u) = 0;

    /**
     * compute (grad T)(x + u(x))
     */
    virtual void transformGradient(const Displacement& u) = 0;

    /**
     * compute T(x + u(x)) and (grad T)(x + u(x))
     */
    virtual void transform(const Displacement &u) = 0;

    /**
     * get the transformed image  T(x + u(x))
     */
    virtual const Array& getTransformedImage() const = 0;

    /**
     * get (grad T)(x + u(x))
     */
    virtual const Array& getTransformedGradient() const = 0;

};
/**
 * \brief Image2D
 * \author Nils Papenberg, Stefan Heldmann
 */
  class Image2D : public virtual Image, public Array2D {
public:
    Image2D():Array2D() {
      //printf("Constructor Image2D()\n\n");
    };

    Image2D( int, int, double, double);

    Image2D( int*, double*);

    /**
     * \brief make Image2D from a Tiff file
     * \author Silke Bommersheim
     */
    Image2D ( char *, double, double);

    /**
     * \brief Copyconstructor
     */
    Image2D( const Image2D&);

    /**
     * \brief Destructor
     */
    virtual ~Image2D() {
      //printf("Destructor Image2D()\n\n");
    };


    /**
     * \brief init-Method
     * Imagedata and also Imagedimensions are read from an Tiff file
     * \author Nils Papenberg
     */
    void init(char *name, double h1, double h2);

    /**
     * \brief init-method for only allocating memory
    */
    virtual void init(int m1, int m2, double h1, double h2);

    /**
     * \brief Write Image2D as a Tiff File named name
     * \author Nils Papenberg
     */
    void writeToTiff(char *name) const;

    /**
     * \brief Read Image2D from a Tiff File named name
     * \author Nils Papenberg
     */
//    #if 0
//    void readFromTiff(char *name);
//    #endif

};



/**
 * \brief TransformableImage2D
 * \author Stefan Heldmann, Nils Papenberg
 */

class TransformableImage2D   : public  Image2D, public TransformableImage {
public:
    /**
     * \brief Constructor
     */
    TransformableImage2D();

    TransformableImage2D( int, int, double, double);

    //MeVis.MK: added init
    virtual void init(int m1, int m2, double h1, double h2);
   /**
    *  \brief make TransformableImage2D from a Tiff file
    *  \author Silke Bommersheim
    */
    TransformableImage2D( char *, double, double);

   /**
    *  \brief Copyconstructor
    */
    TransformableImage2D( const TransformableImage2D&);

    ~TransformableImage2D() {
      //printf("Destructor TransformableImage2D()\n\n");
    };

//#if 0
    /**
     * \brief init-Method
     * Imagedata and also Imagedimensions are given
     * \author Nils Papenberg
     */
//    void init(int m1, int m2, double h1, double h2);
//    void init(int* _dims, double* _h);

    /**
     * \brief init-Method
     * Imagedata and also Imagedimensions are read from an Tiff file
     * \author Nils Papenberg
     */
//    void init(char *name, double* h);

//#endif

    /**
     * \brief init-Method
     * Imagedata and also Imagedimensions are read from an Tiff file
     * \author Nils Papenberg
     */
    void init(char *name, double h1, double h2);


    /**
     * compute transformed image T(x + u(x))
     */
    virtual void transformImage(const Displacement& u);

    /**
     * \brief compute (grad T)(x + u(x))
     */
    virtual void transformGradient(const Displacement& u);

    /**
     * \brief compute T(x + u(x)) and (grad T)(x + u(x))
     */
    virtual void transform(const Displacement& u);

    /**
     * \brief get the transformed image  T(x + u(x))
     */
    virtual const Array& getTransformedImage() const;

    /**
     * \brief get (grad T)(x + u(x))
     */
    virtual const Array& getTransformedGradient() const;

    void writeTransformedImageToTiff(char *name);

private:
    Array2D transformedImage;
    Array3D transformedGradient;

};


/**
 * \brief Image3D
 * \author Nils Papenberg
 */
  class Image3D : public virtual Image, public Array3D {
public:
    Image3D():Array3D() {
      //printf("Constructor Image3D()\n\n");
    };

    Image3D( int, int, int, double, double, double);

    Image3D( int*, double*);

    /**
     * \brief make Image3D from a file
     * \author N. Papenberg, K. Ens
     */
    Image3D ( char *, double, double, double);

    /**
     * \brief Copyconstructor
     */
    Image3D( const Image3D&);

    /**
     * \brief Destructor
     */
    virtual ~Image3D() {
      //printf("Destructor Image3D()\n\n");
    };


    /**
     * \brief init-Method
     * Imagedata and also Imagedimensions are read file
     * have to be implemented !!!
     * \author Konstantin Ens
     */
    void init(char *name, double h1, double h2, double h3);

    /**
     * \brief Write Image3D as a File named name
     * \author Konstantin Ens, Nils Papenberg
     */
    void writeToFile(char *name) const;

    /**
     * \brief Read Image3D from a File named name
     * \author Konstantin Ens, Nils Papenberg
     */
    void readFromFile(char *name);


};

} // namespace

#endif
