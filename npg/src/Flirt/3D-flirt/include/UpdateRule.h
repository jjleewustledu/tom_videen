//	File:	UpdateRule.h

#ifndef _CLASS_UPDATERULE_H_
#define _CLASS_UPDATERULE_H_

#include <stdio.h>

namespace flirt
{
	//#include "DistanceMeasure.h"
	//#include "Smoother.h"
	//#include "Displacement.h"
	//#include "Optimizer.h"

	class DistanceMeasure;
	class Smoother;
	class Optimizer;
	class Displacement;
	class TransformableImage;

	//	Update for the transformation after computing an update with the optimizer

	class UpdateRule
	{
		public:
			
			UpdateRule(DistanceMeasure& _dist,
				Smoother&        _smoother,
				Optimizer&       _opti);

			virtual ~UpdateRule() {};

			// Update u; e.g. by linesarch u = u + lambda*opti.getUpdate()
			virtual void updateDisplacement(Displacement& u);

		protected:
			DistanceMeasure& dist;
			Smoother&        smoother;
			Optimizer&       opti;

	};

	// Update for the transformation after computing an update with the optimizer with a linesearch

	class LineSearch : public UpdateRule
	{
		public:

			LineSearch(DistanceMeasure& dist,
				Smoother&        smoother,
				Optimizer&       opti,
				TransformableImage& TI,
				const Displacement& u);

			virtual void updateDisplacement(Displacement& u);

		private:
			TransformableImage& T;
			int    isFirstUpdate;
			Array  uold;
			double valold;
	};

}
#endif
