#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <boost/math/constants/constants.hpp>

namespace math
{
  static const double deg2rad = boost::math::double_constants::pi / 180.0;
  const double rad2deg = 180.0 / boost::math::double_constants::pi;
  const double z0 = 377.0;
  const double y0 = 1.0 / z0;
  const double c = 3e8;
  const std::complex<double> cj(0.0,1.0);
  const std::complex<double> zero(0.0,0.0);
  const std::complex<double> one(1.0,0.0);
}

#endif // CONSTANTS_H
