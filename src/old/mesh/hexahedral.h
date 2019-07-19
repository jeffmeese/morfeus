#ifndef HEXAHEDRAL_H
#define HEXAHEDRAL_H

#include "morfeus.h"

#include "mesh/element.h"

class Hexahedral
    : public Element
{
public:
  MORFEUS_LIB_DECL Hexahedral(std::size_t id);

public:
  MORFEUS_LIB_DECL virtual dcomplex computeMatrixEntry(const Material * eps, const Material * mu, const Mesh & mesh, double freq, std::size_t source, std::size_t test) const;
  MORFEUS_LIB_DECL virtual std::unique_ptr<Element> copy(size_t newId) const;
  MORFEUS_LIB_DECL virtual std::unique_ptr<Face> createFace(size_t localFace) const;
  MORFEUS_LIB_DECL virtual void getEdgeNodes(std::size_t localEdge, std::size_t & node1, std::size_t & node2) const;
};

#endif // HEXAHEDRAL_H
