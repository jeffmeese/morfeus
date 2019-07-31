#include "functions.h"

#include "constants.h"

namespace morfeus {
namespace math {

void centroid(std::size_t points, double const * x, double const * y, double & xc, double & yc)
{

}

void centerPolygon(size_t numEdges, double * x, double * y)
{
}

double frequencyToWavelength(double frequency)
{
  return (30.0 / frequency);
}

double frequencyToWavenumber(double frequency)
{
  return 2.0*boost::math::double_constants::pi / math::frequencyToWavelength(frequency);
}

double triangleArea(double const * x, double const * y)
{
  return 0.0;
}

double wavelengthToFrequency(double wavelength)
{
  return math::c / wavelength;
}

double wavenumberToFrequency(double wavelength)
{
  return 2.0*boost::math::double_constants::pi / math::wavelengthToFrequency(wavelength);
}

}
}
