#ifndef _CLASS_DISTANCEMEASURE_H_
#define _CLASS_DISTANCEMEASURE_H_

#include <stdio.h>
#include "Image.h"
#include "Array.h"

namespace flirt {

class Image2D;
class TransformableImage2D;
class Array3D;
class Array;

/** \brief The distance measure */
class DistanceMeasure {

public:
    /** \brief Constructor */
    DistanceMeasure(void) {value = 0.0;};

    /** \brief virtual Destructor */
    virtual ~DistanceMeasure() {};


    /** \brief Compute D[R,T;u] */
    virtual void eval() = 0;

    /** \brief Compute dD[R,T;u] */
    virtual void evalGradient() = 0;

    /** \brief get D[R,T;u] */
    double getValue() const;

    /** \brief get S[R,T;u] */
    virtual const Array& getGradient() const = 0;



protected:
    double value;
};

class SumOfSquaredDifferences : public DistanceMeasure {

public:
    /** Constructor */
    SumOfSquaredDifferences(Image& _R, TransformableImage& _T);

    /** Destructor */
    ~SumOfSquaredDifferences() {};


    /** \brief evaluate SSD */
    void eval();

    /** \brief evaluate \grad SSD*/
    void evalGradient();



    /** \brief returns \grad SSD  */
    const Array& getGradient() const;

protected:
    Array3D grad;
    Image& R;
    TransformableImage& T;

};

}

#endif
