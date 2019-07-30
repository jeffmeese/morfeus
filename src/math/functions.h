#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "morfeus.h"

namespace Morfeus {

namespace math
{
  //template <typename T> std::valarray<T> abs(const std::valarray<T> & v);
  //template <typename T> std::valarray<std::complex<T>> conj(const std::valarray<std::complex<T>> & v);
  //template <typename T> math::vector<T> crossProduct(const math::vector<T> & v1, const math::vector<T> & v2);
  //template <typename T> T dotProduct(const math::vector<T> & v1, const math::vector<T> & v2);
  //template <typename T> T innerProduct(const std::valarray<T> & v1, const std::valarray<T> & v2);
  //template <typename T> T norm(const std::valarray<T> & v);

  MORFEUS_LIB_DECL void centroid(std::size_t points, double const * x, double const * y, double & xc, double & yc);
  MORFEUS_LIB_DECL void centerPolygon(size_t numEdges, double * x, double * y);
  MORFEUS_LIB_DECL double frequencyToWavelength(double frequency);
  MORFEUS_LIB_DECL double frequencyToWavenumber(double frequency);
  MORFEUS_LIB_DECL double triangleArea(double const * x, double const * y);
  MORFEUS_LIB_DECL double wavelengthToFrequency(double wavelength);
  MORFEUS_LIB_DECL double wavenumberToFrequency(double wavelength);
}

}

#endif // FUNCTIONS_H
