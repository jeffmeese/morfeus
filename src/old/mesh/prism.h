#ifndef PRISM_H
#define PRISM_H

#include "morfeus.h"

#include "mesh/element.h"

#include <memory>
#include <cstddef>

class Prism
    : public Element
{
public:
  MORFEUS_LIB_DECL Prism(std::size_t id);

public:
  MORFEUS_LIB_DECL virtual dcomplex computeMatrixEntry(const Material * eps, const Material * mu, const Mesh & mesh, double freq, std::size_t source, std::size_t test) const;
  MORFEUS_LIB_DECL virtual std::unique_ptr<Element> copy(size_t newId) const;
  MORFEUS_LIB_DECL virtual std::unique_ptr<Face> createFace(size_t localFace) const;
  MORFEUS_LIB_DECL virtual void getEdgeNodes(std::size_t localEdge, std::size_t & node1, std::size_t & node2) const;

private:
  void edgeConstants(std::size_t edge, std::size_t & k1, std::size_t & k2) const;
  double ixs(double a, double const * x, double const * y) const;
  double iys(double a, double const * x, double const * y) const;
  double ixxs(double a, double b, double const * x, double const * y) const;
  double iyys(double a, double b, double const * x, double const * y) const;
  double izzl(double zl, double zu, double a, double b) const;
  double sxx(double const * x, double const * y) const;
  double sxy(double const * x, double const * y) const;
  double syy(double const * x, double const * y) const;
};

#endif // PRISM_H
