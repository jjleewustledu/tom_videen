#include "Solver.h"

namespace flirt {

  // ---------------------------------------------------------------------
  TMSolverDiffusiveFFT2D :: TMSolverDiffusiveFFT2D() {
    plan = NULL;
    bc   = NEUMANN;
    gs   = GRIDPOINT;
  }

  TMSolverDiffusiveFFT2D :: TMSolverDiffusiveFFT2D( const Displacement2D& u, double _alpha, double _tau)
    : ans( &u.getDimensions()[1], u.getVoxelSize()), rhs( &
      u.getDimensions()[1], u.getVoxelSize()){

    bc   = NEUMANN;
    gs   = GRIDPOINT;
    alpha = _alpha;
    tau = _tau;

    plan = createDeltaPlan2D(u.getDimensions()[1],
                 u.getDimensions()[2],
                 u.getVoxelSize()[0],
                 u.getVoxelSize()[1],
                 bc,
                 gs);

  }

  TMSolverDiffusiveFFT2D :: ~TMSolverDiffusiveFFT2D() {
    deleteDeltaPlan(plan);
  }

  void TMSolverDiffusiveFFT2D :: solve() {
    solveDelta(plan,1.0, -tau*alpha, rhs.getArray()[0][0], ans.getArray()[0][0]);
    solveDelta(plan,1.0, -tau*alpha, rhs.getArray()[1][0], ans.getArray()[1][0]);
  }

  Array& TMSolverDiffusiveFFT2D :: getAns() {
    return ans;
  }

  Array& TMSolverDiffusiveFFT2D :: getRhs() {
    return rhs;
  }

  // ---------------------------------------------------------------------
  TMSolverCurvatureFFT2D :: TMSolverCurvatureFFT2D() {
    plan = NULL;
    bc   = NEUMANN;
    gs   = GRIDPOINT;
  }

  TMSolverCurvatureFFT2D :: TMSolverCurvatureFFT2D( const Displacement2D& u, double _alpha, double _tau)
    : ans( &u.getDimensions()[1], u.getVoxelSize()), rhs( &
      u.getDimensions()[1], u.getVoxelSize()){

    bc   = NEUMANN;
    gs   = GRIDPOINT;
    alpha = _alpha;
    tau = _tau;

    plan = createDeltaPlan2D(u.getDimensions()[1],
                 u.getDimensions()[2],
                 u.getVoxelSize()[0],
                 u.getVoxelSize()[1],
                 bc,
                 gs);

  }

  TMSolverCurvatureFFT2D :: ~TMSolverCurvatureFFT2D() {
    deleteDeltaPlan(plan);
  }

  void TMSolverCurvatureFFT2D :: solve() {
    solveCurvature(plan,1.0, tau*alpha, rhs.getArray()[0][0], ans.getArray()[0][0]);
    solveCurvature(plan,1.0, tau*alpha, rhs.getArray()[1][0], ans.getArray()[1][0]);
  }

  Array& TMSolverCurvatureFFT2D :: getAns() {
    return ans;
  }

  Array& TMSolverCurvatureFFT2D :: getRhs() {
    return rhs;
  }

  FPSolverCurvatureFFT2D :: FPSolverCurvatureFFT2D() {
    plan = NULL;
    bc   = NEUMANN;
    gs   = GRIDPOINT;
  }

  FPSolverCurvatureFFT2D :: FPSolverCurvatureFFT2D( const Displacement2D& u, double _alpha)
    : ans( &u.getDimensions()[1], u.getVoxelSize()), rhs( &
      u.getDimensions()[1], u.getVoxelSize()){
    //printf("Constructor FPSolverCurvatureFFT2D( const Displacement2&)\n\n");
    bc   = NEUMANN;
    gs   = GRIDPOINT;

    plan = createDeltaPlan2D(u.getDimensions()[1],
                 u.getDimensions()[2],
                 u.getVoxelSize()[0],
                 u.getVoxelSize()[1],
                 bc,
                 gs);

    alpha = _alpha;
  }

  FPSolverCurvatureFFT2D :: ~FPSolverCurvatureFFT2D() {
    deleteDeltaPlan(plan);
  }

  void FPSolverCurvatureFFT2D :: solve() {
    solveCurvature(plan,0.0, alpha, rhs.getArray()[0][0], ans.getArray()[0][0]);
    solveCurvature(plan,0.0, alpha, rhs.getArray()[1][0], ans.getArray()[1][0]);
  }

  Array& FPSolverCurvatureFFT2D :: getAns() {
    return ans;
  }

  Array& FPSolverCurvatureFFT2D :: getRhs() {
    return rhs;
  }

  // ---------------------------------------------------------------------
  FPSolverElasticFFT2D :: FPSolverElasticFFT2D() {
    mu     = -1.0;
    lambda = -1.0;
    plan   = NULL;
    bc     = PERIODIC;
  }

  FPSolverElasticFFT2D :: FPSolverElasticFFT2D( const Displacement2D& u, double _alpha, double _mu, double _lambda)
    : ans( &u.getDimensions()[1], u.getVoxelSize()), rhs( &
      u.getDimensions()[1], u.getVoxelSize()){

    bc   = PERIODIC;
    alpha = _alpha;
    mu = _mu;
    lambda = _lambda;

    initElasticPlan2D(u.getDimensions()[1],
              u.getDimensions()[2],
              alpha,
              mu,
              lambda,
              u.getVoxelSize()[0],
              u.getVoxelSize()[1],
              bc,
              &plan);

  }

  FPSolverElasticFFT2D :: ~FPSolverElasticFFT2D() {
    if (plan != NULL)
      deleteElasticPlan2D(plan);
  }

  void FPSolverElasticFFT2D :: solve() {
    //printf("f:\n"); rhs.print();
    solveElastic2D(plan, rhs.getArray(), ans.getArray());
  }

  Array& FPSolverElasticFFT2D :: getAns() {
    return ans;
  }

  Array& FPSolverElasticFFT2D :: getRhs() {
    return rhs;
  }

}
