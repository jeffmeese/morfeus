#ifndef PRISMELEMENT_H
#define PRISMELEMENT_H

#include "morfeus.h"
#include "element.h"

class PrismElement
    : public Element
{
public:
  MORFEUS_LIB_DECL PrismElement();
  MORFEUS_LIB_DECL PrismElement(int32_t number);

protected:
  void doComputeFeEntry(const Mesh * mesh, std::size_t localEdge1, std::size_t localEdge2, dcomplex & i1, dcomplex & i2) const override;
  Face * doConstructFace(std::size_t index) const override;
  void doEdgeNodes(std::size_t index, int32_t & localNode1, int32_t & localNode2) const override;

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

#endif // PRISMELEMENT_H
