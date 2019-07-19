#include "mesh/quadrilateral.h"
#include "mesh/hexahedral.h"

#include "math/constants.h"
#include "math/gaussquad.h"

#include <cmath>

Quadrilateral::Quadrilateral()
  : Face(4,4)
{
}

double Quadrilateral::computeArea(const Mesh & mesh) const
{
  return 1.0;
}

Face::FarFieldEntry Quadrilateral::computeFarFieldEntry(double freq, double theta, double phi, const dcomplex_array & efield, const Mesh & mesh) const
{
	FarFieldEntry entry;
	entry.i1 = math::ZERO;
	entry.i2 = math::ZERO;
	return entry;
}

dcomplex Quadrilateral::computePlanewaveEntry(std::size_t localEdge, double freq, const Planewave * planewave, const Mesh & mesh) const
{
  dcomplex result(math::ZERO);
  return result;
}

std::unique_ptr<Element> Quadrilateral::extrudeElement(std::size_t id, std::size_t nodeOffset) const
{
  std::unique_ptr<Hexahedral> hex = std::make_unique<Hexahedral>(id);
  return std::move(hex);
}
