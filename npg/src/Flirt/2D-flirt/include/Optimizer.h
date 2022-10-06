#ifndef _CLASS_OPTIMIZER_H_
#define _CLASS_OPTIMIZER_H_

#include <stdio.h>

namespace flirt {

class Array;
class DistanceMeasure;
class TMSolver;
class FPSolver;
class Displacement;
class Smoother;

/** 
 * \brief Abstract base class for any Optimizer
 * \author Stefan Heldmann, Nils Papenberg
 */
class Optimizer {
public:
  virtual void computeDisplacementUpdate(Displacement&) = 0;
  virtual Array& getUpdate() = 0;
};


/** 
 * \brief Time marching optimizer.
 * computes \f$ (I+\tau\alpha A) u_{\mathrm{update}} = u-\tau f  \f$
 * \author Stefan Heldmann, Nils Papenberg
 */
class TMOptimizer : public Optimizer {
public:
  TMOptimizer(DistanceMeasure&, Smoother&, TMSolver&);
  TMOptimizer(DistanceMeasure&, Smoother&, TMSolver&, double, double);
  
  void setParameter(double, double);
  void computeDisplacementUpdate(Displacement& u);
  Array& getUpdate();
  
  
 protected:
  TMSolver& solver;
  DistanceMeasure& distance;
  Smoother& smoother;
  double alpha;
  double tau;
};

// --------------------------------------------------------

/** 
 * \brief Fixpoint optimizer.
 * computes \f$ \alpha A u_{\mathrm{update}} = \tau f  \f$
 * \author Stefan Heldmann
 */
class FPOptimizer : public Optimizer {
public:
  FPOptimizer(DistanceMeasure&, Smoother&, FPSolver&);
  FPOptimizer(DistanceMeasure&, Smoother&, FPSolver&, double);
  
  void setAlpha(double _alpha);
  void computeDisplacementUpdate(Displacement& u);
  double getAlpha(){return alpha;};
  Array& getUpdate();
  
  
protected:
  FPSolver& solver;
  DistanceMeasure& distance;
  Smoother& smoother;
  double alpha;
};

}

#endif
