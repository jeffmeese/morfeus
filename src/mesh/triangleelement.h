#ifndef TRIANGLEELEMENT_H
#define TRIANGLEELEMENT_H

#include "morfeus.h"
#include "element.h"

class TriangleElement
    : public Element
{
public:
  MORFEUS_LIB_DECL TriangleElement(int32_t id);

protected:
  void doEdgeNodes(std::size_t index, int32_t & localNode1, int32_t & localNode2) const override;
  void doComputeFeEntry(const Mesh * mesh, std::size_t localEdge1, std::size_t localEdge2, dcomplex & i1, dcomplex & i2) const override;
  Face * doConstructFace(std::size_t index) const override;
};

#endif // TRIANGLEELEMENT_H
