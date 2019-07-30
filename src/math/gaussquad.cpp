#include "gaussquad.h"

namespace Morfeus {

size_t math::gaussQuadTriangle(size_t numPoints, std::vector<math::TrianglePoint> & points)
{
	if (numPoints == 2) numPoints = 3;
	if (numPoints == 5) numPoints = 6;
	if (numPoints > 7) numPoints = 7;

	points.resize(numPoints);

	if (numPoints == 1) {
		points[0].abs1 = 0.333333333333333;
		points[0].abs2 = 0.333333333333333;
		points[0].abs3 = 0.333333333333333;
		points[0].weight = 1.0;
	}

	if (numPoints == 3) {
		points[0].abs1 = 0.5;
		points[0].abs2 = 0.5;
		points[0].abs3 = 0.0;
		points[0].weight = 0.333333333333333;

		points[1].abs1 = 0.0;
		points[1].abs2 = 0.5;
		points[1].abs3 = 0.5;
		points[1].weight = 0.333333333333333;

		points[2].abs1 = 0.5;
		points[2].abs2 = 0.0;
		points[2].abs3 = 0.5;
		points[2].weight = 0.333333333333333;
	}

	if (numPoints == 4) {
		points[0].abs1 = 0.333333333333333;
		points[0].abs2 = 0.333333333333333;
		points[0].abs3 = 0.333333333333333;
		points[0].weight = -0.56250;

		points[1].abs1 = 0.6;
		points[1].abs2 = 0.2;
		points[1].abs3 = 0.2;
		points[1].weight = 0.520833333333333;

		points[2].abs1 = 0.2;
		points[2].abs2 = 0.6;
		points[2].abs3 = 0.2;
		points[2].weight = 0.520833333333333;

		points[3].abs1 = 0.2;
		points[3].abs2 = 0.2;
		points[3].abs3 = 0.6;
		points[3].weight = 0.520833333333333;
	}

	if (numPoints == 6) {
		//
		// 6-point coordinates and weights
		//
		points[0].abs1 = 0.816847572980459;
		points[0].abs2 = 0.091576213509771;
		points[0].abs3 = 0.091576213509771;
		points[0].weight = 0.109951743655322;

		points[1].abs1 = 0.091576213509771;
		points[1].abs2 = 0.816847572980459;
		points[1].abs3 = 0.091576213509771;
		points[1].weight = 0.109951743655322;

		points[2].abs1 = 0.091576213509771;
		points[2].abs2 = 0.091576213509771;
		points[2].abs3 = 0.816847572980459;
		points[2].weight = 0.109951743655322;

		points[3].abs1 = 0.108103018168070;
		points[3].abs2 = 0.445948490915965;
		points[3].abs3 = 0.445948490915965;
		points[3].weight = 0.223381589678011;

		points[4].abs1 = 0.445948490915965;
		points[4].abs2 = 0.108103018168070;
		points[4].abs3 = 0.445948490915965;
		points[4].weight = 0.223381589678011;

		points[5].abs1 = 0.445948490915965;
		points[5].abs2 = 0.445948490915965;
		points[5].abs3 = 0.108103018168070;
		points[5].weight = 0.223381589678011;
	}

	if (numPoints == 7) {
		points[0].abs1 = 0.333333333333333;
		points[0].abs2 = 0.333333333333333;
		points[0].abs3 = 0.333333333333333;
		points[0].weight = 0.22503300003300000;

		points[1].abs1 = 0.470142064105115;
		points[1].abs2 = 0.470142064105115;
		points[1].abs3 = 0.059715871789770;
		points[1].weight = 0.132394152788506;

		points[2].abs1 = 0.059715871789770;
		points[2].abs2 = 0.470142064105115;
		points[2].abs3 = 0.470142064105115;
		points[2].weight = 0.132394152788506;

		points[3].abs1 = 0.470142064105115;
		points[3].abs2 = 0.059715871789770;
		points[3].abs3 = 0.470142064105115;
		points[3].weight = 0.132394152788506;

		points[4].abs1 = 0.797426985353087;
		points[4].abs2 = 0.101286507323456;
		points[4].abs3 = 0.101286507323456;
		points[4].weight = 0.125939180544827;

		points[5].abs1 = 0.101286507323456;
		points[5].abs2 = 0.797426985353087;
		points[5].abs3 = 0.101286507323456;
		points[5].weight = 0.125939180544827;

		points[6].abs1 = 0.101286507323456;
		points[6].abs2 = 0.101286507323456;
		points[6].abs3 = 0.797426985353087;
		points[6].weight = 0.125939180544827;
	}

	return numPoints;
}

}
