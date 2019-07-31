#ifndef MORFEUS_MATH_FUNCTIONS_H
#define MORFEUS_MATH_FUNCTIONS_H

#include "morfeus.h"

namespace morfeus {
namespace math {

  MORFEUS_LIB_DECL void centroid(std::size_t points, double const * x, double const * y, double & xc, double & yc);
  MORFEUS_LIB_DECL void centerPolygon(size_t numEdges, double * x, double * y);
  MORFEUS_LIB_DECL double frequencyToWavelength(double frequency);
  MORFEUS_LIB_DECL double frequencyToWavenumber(double frequency);
  MORFEUS_LIB_DECL double triangleArea(double const * x, double const * y);
  MORFEUS_LIB_DECL double wavelengthToFrequency(double wavelength);
  MORFEUS_LIB_DECL double wavenumberToFrequency(double wavelength);

}
}

#endif // MORFEUS_MATH_FUNCTIONS_H
