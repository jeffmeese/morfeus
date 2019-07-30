#include "functions.h"

#include "constants.h"

namespace morfeus {

void math::centroid(std::size_t points, double const * x, double const * y, double & xc, double & yc)
{

}

void math::centerPolygon(size_t numEdges, double * x, double * y)
{
}

double math::frequencyToWavelength(double frequency)
{
  return (30.0 / frequency);
}

double math::frequencyToWavenumber(double frequency)
{
  return 2.0*boost::math::double_constants::pi / math::frequencyToWavelength(frequency);
}

double math::triangleArea(double const * x, double const * y)
{
  return 0.0;
}

double math::wavelengthToFrequency(double wavelength)
{
  return math::c / wavelength;
}

double math::wavenumberToFrequency(double wavelength)
{
  return 2.0*boost::math::double_constants::pi / math::wavelengthToFrequency(wavelength);
}

}
