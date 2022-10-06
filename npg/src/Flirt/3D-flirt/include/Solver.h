//	File:	Solver.h
//	Author:	Heldmann, Nils Papenberg

#ifndef _CLASS_SOLVER_H_
#define _CLASS_SOLVER_H_

#include <stdio.h>
#include <flirt.h>
#include <Displacement.h>

namespace       flirt
{

	class Solver
	{
		public:
			virtual void    solve () = 0;
			virtual         Array & getAns () = 0;
			virtual         Array & getRhs () = 0;
	};

	// Abstract class for fix point type schemes -- must be inherited by any solver
	// Solves the LS \ f$ \ alpha A u = f \ f$

	class FPSolver:public Solver
	{
		public:
			FPSolver ()
				: alpha (-1.0) {
			};

			~FPSolver () {}

			virtual void    setAlpha (double a) {
				alpha = a;
			};

			double          getAlpha () const
			{
				return alpha;
			};

		protected:
			double          alpha;
	};

	// FPSolverElasticFFT3D -- solves the eqn:
	// 		alpha A u = f		with:
	// 		A = -(\mu\Delta+(\lambda+\mu)\nabla\mathrm{div})

	class FPSolverElasticFFT3D:public FPSolver
	{
		public:
			FPSolverElasticFFT3D ();

			FPSolverElasticFFT3D (const Displacement3D&, double, double, double);

			~FPSolverElasticFFT3D ();

			virtual void    setMu (double m) {
				mu = m;
			};

			double          getMu () const {
				return mu;
			};

			virtual void    setLambda (double l) {
				lambda = l;
			};

			double          getLambda () const {
				return lambda;
			};

			virtual void    setParameter (double _alpha, double _mu, double _lambda) {
				alpha = _alpha;
				mu = _mu;
				lambda = _lambda;
			};

			virtual void    solve ();
			virtual         Array & getAns ();
			virtual         Array & getRhs ();

		protected:
			double          mu;
			double          lambda;
			Displacement3D  ans;
			Displacement3D  rhs;
			elasticPlan3D   plan;
			boundary_t      bc;

	};
}
#endif
