#ifndef MATH_CONSTANTS_H
#define MATH_CONSTANTS_H

#include <cmath>
#include <complex>

namespace math
{
	const double PI = 4.0*atan(1.0);
	const double DEG_TO_RAD = math::PI / 180.0;
	const double RAD_TO_DEG = 180.0 / math::PI;
	const double Z0 = 377.0;
	const double Y0 = 1.0 / Z0;
	const double c = 3e8;
	const std::complex<double> CJ(0.0,1.0);
	const std::complex<double> ZERO(0.0,0.0);
	const std::complex<double> ONE(1.0,0.0);
}

#endif // #ifndef MATH_CONSTANTS_H
