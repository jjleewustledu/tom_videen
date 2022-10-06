#ifndef _CLASS_DISTANCEMEASURE_H_
#define _CLASS_DISTANCEMEASURE_H_

#include <stdio.h>
#include "Image.h"
#include "Array.h"

namespace flirt
{
	class Image2D;
	class TransformableImage2D;
	class Array3D;
	class Array;

	class DistanceMeasure
	{
		public:
			DistanceMeasure() {};

			virtual ~DistanceMeasure() {};

			// Compute D[R,T;u]
			virtual void eval() = 0;

			// Compute dD[R,T;u]
			virtual void evalGradient() = 0;

			// Get D[R,T;u]
			double getValue() const;

			// Get S[R,T;u]
			virtual const Array& getGradient() const = 0;

		protected:
			double value;
	};

	class SumOfSquaredDifferences : public DistanceMeasure
	{
		public:
			SumOfSquaredDifferences(Image& _R, TransformableImage& _T);

			~SumOfSquaredDifferences() {};

			// Evaluate SSD
			void eval();

			// Evaluate grad SSD
			void evalGradient();

			// Returns grad SSD
			const Array& getGradient() const;

		protected:
			Array4D grad;
			Image& R;
			TransformableImage& T;

	};

}
#endif
