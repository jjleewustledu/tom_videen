#ifndef _CLASS_SOLVER_H_
#define _CLASS_SOLVER_H_

#include <stdio.h>
#include "flirt.h"
#include "Displacement.h"

namespace flirt {

/**   
 * Abstract class. Must be inherited by any solver.
 * \author Stefan Heldmann, Nils Papenberg
 */
class Solver {
public:
	virtual void   solve()  = 0;
	virtual Array& getAns() = 0;
	virtual Array& getRhs() = 0;	
};

// --------------------------------------------------------------------
// --------------------------------------------------------------------
// --------------------------------------------------------------------
// --------------------------------------------------------------------
// --------------------------------------------------------------------
// --------------------------------------------------------------------
// --------------------------------------------------------------------
// --------------------------------------------------------------------
// --------------------------------------------------------------------
// --------------------------------------------------------------------


/**   
 * Abstract class for fix point type schemes.
 * Must be inherited by any solver.
 * Solves the LS \f$ \alpha A u = f \f$ 
 * \author Stefan Heldmann, Nils Papenberg
 */
class FPSolver : public Solver {
public:
	FPSolver() : alpha(-1.0) {};
	~FPSolver() {};
	virtual void setAlpha(double a) { alpha = a; };
	double       getAlpha() const { return alpha; };
	
protected:
        double alpha;
};

// --------------------------------------------------------------------

/**   
 * Solves the biharmonic equation \f$ \alpha \Delta^2 u = f \f$ 
 * \author Stefan Heldmann
 * \date 27-07-2005
 */
/**
class FPSolverCurvatureFFT2D : public FPSolver {
public: 
	FPSolverCurvatureFFT2D(); 
        FPSolverCurvatureFFT2D( const Displacement2D&, double);
	~FPSolverCurvatureFFT2D();  
	
	virtual void solve();
	virtual Array& getAns();
	virtual Array& getRhs();
	
protected:
	Displacement2D ans;
	Displacement2D rhs;
	deltaPlan plan;
	
	boundary_t bc;
	gridsymmetry_t gs;
	
};
*/
// --------------------------------------------------------------------

/**   
 * Solves the equation
 * \f$ \alpha A u = f \f$ with 
 * \f$ A = -(\mu\Delta+(\lambda+\mu)\nabla\mathrm{div}) \f$ 
 * \author Stefan Heldmann
 * \date 27-07-2005
 */
/*
class FPSolverElasticFFT2D : public FPSolver {
public:
	FPSolverElasticFFT2D();
	FPSolverElasticFFT2D( const Displacement2D&, double, double, double);
	~FPSolverElasticFFT2D();
	
	
	virtual void setMu(double m) { mu = m; };
	double       getMu() const { return mu; };
	
	virtual void setLambda(double l) { lambda = l; };
	double       getLambda() const { return lambda; };
	
	
	virtual void setParameter(double _alpha, double _mu, double _lambda) {
		alpha  = _alpha;
		mu     = _mu;
		lambda = _lambda;
	};

	virtual void solve();
	virtual Array& getAns();
	virtual Array& getRhs();

protected:
	double mu;
	double lambda;
	Displacement2D ans;
	Displacement2D rhs;
	elasticPlan plan;
	boundary_t bc;
	
};
*/
//============================
// FPSolverElasticFFT3D
//============================
// --------------------------------------------------------------------

/**   
 * Solves the equation
 * \f$ \alpha A u = f \f$ with 
 * \f$ A = -(\mu\Delta+(\lambda+\mu)\nabla\mathrm{div}) \f$ 
 * \author Konstantin Ens
 * \date 24-04-2006
 */
class FPSolverElasticFFT3D : public FPSolver {
public:
	FPSolverElasticFFT3D();
	FPSolverElasticFFT3D( const Displacement3D&, double, double, double);
	~FPSolverElasticFFT3D();
	
	
	virtual void setMu(double m) { mu = m; };
	double       getMu() const { return mu; };
	
	virtual void setLambda(double l) { lambda = l; };
	double       getLambda() const { return lambda; };
	
	
	virtual void setParameter(double _alpha, double _mu, double _lambda) {
		alpha  = _alpha;
		mu     = _mu;
		lambda = _lambda;
	};

	virtual void solve();
	virtual Array& getAns();
	virtual Array& getRhs();

protected:
	double mu;
	double lambda;
	Displacement3D ans;
	Displacement3D rhs;
	elasticPlan3D plan;
	boundary_t bc;
	
};

// --------------------------------------------------------------------
//============================
// Ende des FPSolverElasticFFT3D
//============================
// --------------------------------------------------------------------
// --------------------------------------------------------------------
// --------------------------------------------------------------------
// --------------------------------------------------------------------
// --------------------------------------------------------------------
// --------------------------------------------------------------------
// --------------------------------------------------------------------
// --------------------------------------------------------------------
// --------------------------------------------------------------------
// --------------------------------------------------------------------

/**   
 * Abstract class for time marching schemes.
 * Must be inherited by any solver.
 * Solves the LS \f$ (I+\tau\alpha A) u = f \f$ 
 * \author Stefan Heldmann, Nils Papenberg
 */
/*
class TMSolver : public Solver {
public:
	TMSolver() : alpha(-1.0), tau(-1.0) {};
	~TMSolver() {};
	
	virtual void setAlpha(double a) { alpha = a; };
	double       getAlpha() const { return alpha; };
	
	virtual void setTau(double t) { tau = t; };
	double       getTau() const { return alpha; };
	
	
	virtual void setParameter(double _alpha, double _tau) {
		alpha = _alpha;
		tau = _tau;
	};

protected:
	double alpha;
	double tau;
};
*/
// --------------------------------------------------------------------

/**   
 * Time marching solver for diffusive registration. 
 * Solves the LS \f$ (I-\tau\alpha \Delta) u = f \f$. 
 * Solving is done with AOS (Additive Operator Splitting)
 * \author Nils Papenberg
 */
/*
class TMSolverDiffusiveAOS2D : public TMSolver {
public:
	TMSolverDiffusiveAOS2D();
	TMSolverDiffusiveAOS2D( const Displacement2D&, double, double);
	~TMSolverDiffusiveAOS2D();

	virtual void solve();
	virtual Array& getAns();
	virtual Array& getRhs();

protected:
	Displacement2D ans;
	Displacement2D rhs;
	diffusivePlan diffMatrix;
	boundary_t bc;
	
};
*/
// --------------------------------------------------------------------



/**   
 * Time marching solver for diffusive registration. 
 * Solves the LS \f$ (I-\tau\alpha \Delta) u = f \f$. 
 * Solving is done with FFT 
 * \author Stefan Heldmann
 */
/*
class TMSolverDiffusiveFFT2D : public TMSolver {
public:
	TMSolverDiffusiveFFT2D();  
	TMSolverDiffusiveFFT2D( const Displacement2D&, double, double);
	~TMSolverDiffusiveFFT2D();  
	
	virtual void solve();
	virtual Array& getAns();
	virtual Array& getRhs();

protected:
	Displacement2D ans;
	Displacement2D rhs;
	deltaPlan plan;	
	boundary_t bc;
	gridsymmetry_t gs;
	
};
*/
// --------------------------------------------------------------------

/**   
 * Time marching solver for curvature registration. 
 * Solves the LS \f$ (I+\tau\alpha \Delta^2) u = f \f$. 
 * Solving is done with FFT 
 * \author Stefan Heldmann
 */
/*
class TMSolverCurvatureFFT2D : public TMSolver {
public:
	TMSolverCurvatureFFT2D();  
	TMSolverCurvatureFFT2D( const Displacement2D&, double, double);
	~TMSolverCurvatureFFT2D();  
	
	virtual void solve();
	virtual Array& getAns();
	virtual Array& getRhs();
	

protected:
	Displacement2D ans;
	Displacement2D rhs;
	deltaPlan plan;
	boundary_t bc;
	gridsymmetry_t gs;
	
};
*/
// --------------------------------------------------------------------

}

#endif
