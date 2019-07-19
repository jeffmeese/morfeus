#ifndef QUADRILATERAL_H
#define QUADRILATERAL_H

#include "morfeus.h"

#include "mesh/face.h"

class Quadrilateral
    : public Face
{
public:
  MORFEUS_LIB_DECL Quadrilateral();

public:
  MORFEUS_LIB_DECL virtual FarFieldEntry computeFarFieldEntry(double freq, double theta, double phi, const dcomplex_array & efield, const Mesh & mesh) const;
  MORFEUS_LIB_DECL virtual dcomplex computePlanewaveEntry(std::size_t localEdge, double freq, const Planewave * planewave, const Mesh & mesh) const;
  MORFEUS_LIB_DECL virtual std::unique_ptr<Element> extrudeElement(std::size_t id, std::size_t nodeOffset) const;
  MORFEUS_LIB_DECL virtual double computeArea(const Mesh & mesh) const;
};

#endif // QUADRILATERAL_H
