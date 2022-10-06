//	File:	Optimizer.h
//	Author:	Stefan Heldmann, Nils Papenberg

#ifndef _CLASS_OPTIMIZER_H_
#define _CLASS_OPTIMIZER_H_

#include <stdio.h>

namespace flirt
{
	class Array;
	class DistanceMeasure;
	class TMSolver;
	class FPSolver;
	class Displacement;
	class Smoother;

	// Abstract base class for any Optimizer

	class Optimizer
	{
		public:
			virtual void computeDisplacementUpdate(Displacement&) = 0;
			virtual Array& getUpdate() = 0;
	};

	// Time marching optimizer.
	// Computes \f$ (I+\tau\alpha A) u_{\mathrm{update}} = u-\tau f  \f$

	class TMOptimizer : public Optimizer
	{
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

	// Fixpoint optimizer.
	// Computes \f$ \alpha A u_{\mathrm{update}} = \tau f  \f$

	class FPOptimizer : public Optimizer
	{
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
