#ifndef _CLASS_UPDATERULE_H_
#define _CLASS_UPDATERULE_H_

#include <stdio.h>

namespace flirt {

// #include "DistanceMeasure.h"
// #include "Smoother.h"
// #include "Displacement.h"
// #include "Optimizer.h"

class DistanceMeasure;
class Smoother;
class Optimizer;
class Displacement;
class TransformableImage;

/** 
 * \brief Update for the transformation after computing 
 * an update with the optimizer  
 */
class UpdateRule {
 public:
  /** 
   * \brief Constructor 
   */
  UpdateRule(DistanceMeasure& _dist, 
	     Smoother&        _smoother,
	     Optimizer&       _opti);
  
  /** 
   * \brief Destructor 
   */
  virtual ~UpdateRule() {
   //   printf("UpdateRule (%p) :: Destructor\n", this);
  };
  
  /** 
   * \brief update u; e.g. by linesarch u = u + lambda*opti.getUpdate() 
   */

  virtual void updateDisplacement(Displacement& u); 
    
 protected:
  DistanceMeasure& dist;
  Smoother&        smoother;
  Optimizer&       opti;
  
};


/** 
 * \brief Update for the transformation after computing 
 *  an update with the optimizer with a linesearch  
 */
class LineSearch : public UpdateRule {
 public:
  /** 
   * \brief Constructor 
   */
  LineSearch(DistanceMeasure& dist, 
	     Smoother&        smoother,
	     Optimizer&       opti,
	     TransformableImage& TI,
             const Displacement2D& u);

  virtual void updateDisplacement(Displacement& u); 
  
 private:
  TransformableImage& T;
  int    isFirstUpdate;
  Array  uold;
  double valold;
};

}

#endif
