#include "math/functions.h"
#include "math/constants.h"

void math::centroid(std::size_t points, double const * x, double const * y, double & xc, double & yc)
{
	xc = yc = 0.0;
	for (std::size_t i = 0; i < points; i++)
	{
		xc += x[i];
		yc += y[i];
	}
	xc /= points;
	yc /= points;
}

void math::centerPolygon(size_t numSides, double * x, double * y)
{
  // Center the polygon at the origin
  double xc = 0.0, yc = 0.0;
  for (std::size_t i = 0; i < numSides; i++) {
    xc += x[i];
    yc += y[i];
  }

  xc /= numSides;
  yc /= numSides;

  for (std::size_t i = 0; i < numSides; i++) {
    x[i] -= xc;
    y[i] -= yc;
  }
}

double math::frequencyToWavelength(double frequency)
{
  return (30.0 / frequency);
  //return (math::c / frequency);
}

double math::frequencyToWaveNumber(double frequency)
{
  return 2.0*math::PI / math::frequencyToWavelength(frequency);
}

double math::wavelengthToFrequency(double wavelength)
{
  return math::c / wavelength;
}

double math::waveNumberToFrequency(double wavelength)
{
  return 2.0*math::PI / math::wavelengthToFrequency(wavelength);
}

double math::triangleArea(double const * x, double const * y)
{
  double p1 = x[1]*(y[2] - y[0]);
  double p2 = x[0]*(y[2] - y[1]);
  double p3 = x[2]*(y[1] - y[0]);
  return (p1 - p2 - p3) / 2.0;
}

//std::valarray<dcomplex> math::conj(const std::valarray<dcomplex> & v)
//{
//    std::valarray<dcomplex> r;
//    r.resize(v.size());
//    for (size_t i = 0; i < v.size(); i++)
//        r[i] = std::conj(v[i]);
//        
//    return r;
//}
//    
//std::valarray<scomplex> math::conj(const std::valarray<scomplex> & v)
//{
//    std::valarray<scomplex> r;
//    r.resize(v.size());
//    for (size_t i = 0; i < v.size(); i++)
//        r[i] = std::conj(v[i]);
//        
//    return r;
//}
