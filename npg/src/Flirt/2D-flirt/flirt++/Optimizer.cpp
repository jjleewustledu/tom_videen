#include "Optimizer.h"
#include "Array.h"
#include "Solver.h"
#include "DistanceMeasure.h"
#include "Displacement.h"

namespace flirt {
// ---------------------------------------------------------------------
//
// TMOptimizer
//
// ---------------------------------------------------------------------

TMOptimizer :: TMOptimizer(DistanceMeasure& _distance, Smoother& _smoother, TMSolver& _solv)
    : solver(_solv),
    distance(_distance),
    smoother(_smoother),
    alpha(-1.0),
    tau(-1.0)
  {};

TMOptimizer :: TMOptimizer(DistanceMeasure& _distance, Smoother& _smoother, TMSolver& _solv, double _alpha, double _tau)
    : solver(_solv),
    distance(_distance),
    smoother(_smoother),
    alpha(_alpha),
    tau(_tau)
  {};

void TMOptimizer :: setParameter(double _alpha, double _tau) {
  alpha = _alpha;
  tau = _tau;
};

// ---------------------------------------------------------------------
Array& TMOptimizer :: getUpdate() {
  return solver.getAns();
};
// ---------------------------------------------------------------------
void TMOptimizer :: computeDisplacementUpdate(Displacement& u) {
  Array& rhs       = solver.getRhs();

  const Array& f = distance.getGradient();
  int endloop = rhs.getNumberOfElements();

  for (int k = 0; k < endloop; k++) {
    rhs.getData()[k] = u.getData()[k] - tau * f.getData()[k];
  }

  solver.solve();

}



// ---------------------------------------------------------------------
//
// FPOptimizer
//
// ---------------------------------------------------------------------

FPOptimizer ::  FPOptimizer(DistanceMeasure& _distance, Smoother& _smoother, FPSolver& _solv)
  : solver(_solv),
    distance(_distance),
    smoother(_smoother),
    alpha(-1.0){
  //printf("Constructor FPOptimizer(DistanceMeasure&, Smoother&, FPSolver&)\n\n");
}


FPOptimizer ::  FPOptimizer(DistanceMeasure& _distance, Smoother& _smoother, FPSolver& _solv, double _alpha)
  : solver(_solv),
    distance(_distance),
    smoother(_smoother),
    alpha(_alpha)
{}

// ---------------------------------------------------------------------

void FPOptimizer :: setAlpha(double _alpha) {
  alpha = _alpha;
}
// ---------------------------------------------------------------------
Array& FPOptimizer :: getUpdate() {
  return solver.getAns();
}
// ---------------------------------------------------------------------
void FPOptimizer :: computeDisplacementUpdate(Displacement& u) {
  Array& rhs       = solver.getRhs();
  const Array& f = distance.getGradient();
  int endloop = rhs.getNumberOfElements();

  for (int k = 0; k < endloop; k++) {
    rhs.getData()[k] = - f.getData()[k];
  }

  solver.solve();

}

}
// ---------------------------------------------------------------------

