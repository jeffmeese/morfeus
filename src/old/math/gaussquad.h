#ifndef GAUSSQUAD_H
#define GAUSSQUAD_H

#include "morfeus.h"

#include <vector>

namespace math {

	struct TrianglePoint
	{
		double abs1;
		double abs2;
		double abs3;
		double weight;
	};

	MORFEUS_LIB_DECL size_t gaussQuadTriangle(std::size_t numPoints, std::vector<TrianglePoint> & points);
}


#endif // #ifndef GAUSSQUAD_H
