#include "flirt++.h"
namespace flirt {
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
double DistanceMeasure :: getValue() const {
    return value;
};


// -------------------------------------------------------------------------
// SumOfSquaredDifferences
// -------------------------------------------------------------------------
SumOfSquaredDifferences :: SumOfSquaredDifferences(Image& refR, TransformableImage& refT)
  : R(refR), T(refT), grad( 2, refT.getDimensions()[0],refT.getDimensions()[1]) {
  //printf("Constructor SumOfSquaredDifferneces( Image&, TransformableImage&)\n\n");
  value = 0.0;

  /*
     grad.init(2,refT.getDimensions()[0],refT.getDimensions()[1]);
  */

};

// -------------------------------------------------------------------------
void SumOfSquaredDifferences :: eval() {
   // calculate the normalized SSD: 1/N*||T(x-u) - R||_2
   double N = (double)T.getNumberOfElements();
   value = dist2(T.getTransformedImage().getData(),R.getData(),T.getNumberOfElements()) / N;
};

// -------------------------------------------------------------------------
void SumOfSquaredDifferences :: evalGradient() {
    // copy
  sub(T.getTransformedImage().getData(), R.getData(), grad[0][0], T.getNumberOfElements());
  sub(T.getTransformedImage().getData(), R.getData(), grad[1][0], T.getNumberOfElements());
  dot(grad.getData(), T.getTransformedGradient().getData(), grad.getData(), grad.getNumberOfElements());
};

// -------------------------------------------------------------------------
const Array& SumOfSquaredDifferences :: getGradient() const {
    return grad;
};

// -------------------------------------------------------------------------
}
