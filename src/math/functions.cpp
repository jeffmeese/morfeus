#include "functions.h"

#include "constants.h"

double math::frequencyToWavelength(double frequency)
{
  return (30.0 / frequency);
}

double math::frequencyToWavenumber(double frequency)
{
  return 2.0*boost::math::double_constants::pi / math::frequencyToWavelength(frequency);
}

double math::wavelengthToFrequency(double wavelength)
{
  return math::c / wavelength;
}

double math::wavenumberToFrequency(double wavelength)
{
  return 2.0*boost::math::double_constants::pi / math::wavelengthToFrequency(wavelength);
}
