#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H

#include "types.h"
#include "morfeus.h"

#include "math/vector.h"

#include <cassert>

namespace math
{
  typedef std::complex<float> fcomplex;
  typedef std::complex<double> dcomplex;

  template <typename T> std::valarray<T> abs(const std::valarray<T> & v);
  template <typename T> std::valarray<std::complex<T>> conj(const std::valarray<std::complex<T>> & v);
  template <typename T> math::vector<T> crossProduct(const math::vector<T> & v1, const math::vector<T> & v2);
  template <typename T> T dotProduct(const math::vector<T> & v1, const math::vector<T> & v2);
  template <typename T> T innerProduct(const std::valarray<T> & v1, const std::valarray<T> & v2);
  template <typename T> T norm(const std::valarray<T> & v);

  MORFEUS_LIB_DECL void centroid(std::size_t points, double const * x, double const * y, double & xc, double & yc);
  MORFEUS_LIB_DECL void centerPolygon(size_t numEdges, double * x, double * y);
  MORFEUS_LIB_DECL double frequencyToWavelength(double frequency);
  MORFEUS_LIB_DECL double frequencyToWaveNumber(double frequency);
  MORFEUS_LIB_DECL double triangleArea(double const * x, double const * y);
  MORFEUS_LIB_DECL double wavelengthToFrequency(double wavelength);
  MORFEUS_LIB_DECL double waveNumberToFrequency(double wavelength);
}

template <typename T>
inline std::valarray<std::complex<T>> math::conj(const std::valarray<std::complex<T>> & v)
{
    std::valarray<std::complex<T>> r(v.size());
    for (size_t i = 0; i < v.size(); i++)
        r[i] = std::conj(v[i]);

    return r;
}

template <typename T>
inline T math::dotProduct(const math::vector<T> & v1, const math::vector<T> & v2)
{
  return (v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z());
}

template <typename T>
inline math::vector<T> math::crossProduct(const math::vector<T> & v1, const math::vector<T> & v2)
{
  T x = (v1.y()*v2.z() - v1.z()*v2.y());
  T y = (v1.z()*v2.x() - v1.x()*v2.z());
  T z = (v1.x()*v2.y() - v1.y()*v2.x());
  return math::vector<T>(x, y, z);
}

template <typename T>
inline T math::innerProduct(const std::valarray<T> & v1, const std::valarray<T> & v2)
{
  assert(v1.size() == v2.size());

  T result = T();
  for (size_t i = 0; i < v1.size(); i++)
    result += v1[i]*v2[i];

  return result;
}

template <typename T> T math::norm(const std::valarray<T> & v)
{
  return math::innerProduct(v,v);
}

template <typename T>
inline std::valarray<T> math::abs(const std::valarray<T> & v)
{
  std::valarray<T> r(v.size());
  for (size_t i = 0; i < v.size(); i++)
    r[i] = std::abs(v[i]);

  return r;
}

#endif // #ifndef MATH_FUNCTIONS_H
