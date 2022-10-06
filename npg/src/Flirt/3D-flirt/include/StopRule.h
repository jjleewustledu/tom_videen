//	File:	StopRule.h
//	Check for terminition

#ifndef _CLASS_STOPRULE_H_
#define _CLASS_STOPRULE_H_

#include <stdio.h>
#include "DistanceMeasure.h"
#include "Smoother.h"
#include "Array.h"
#include "Displacement.h"

namespace flirt
{

	class StopRule
	{
		DistanceMeasure& D;
		Smoother&        S;
		int              maxIter;

		public:

			// Constructor
			StopRule(DistanceMeasure& DM,
				Smoother& SM,
				int MaxIter)
				: D(DM), S(SM), maxIter(MaxIter) {};

			virtual ~StopRule() {};

			// update u; e.g. by linesarch u = u + lambda*opti.getUpdate()

			virtual bool stopIteration(Displacement& u, int iter) {
				if (iter >= maxIter) return 1; else return 0;
			};

	};

}
#endif
