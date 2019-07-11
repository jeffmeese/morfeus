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
  void doEdgeNodes(int32_t index, int32_t & localNode1, int32_t & localNode2) const override;
  dcomplex doComputeFeEntry(const Mesh & mesh, int32_t localEdge1, int32_t localEdge2) const override;
  Face * doConstructFace(int32_t index) const override;
};

#endif // TRIANGLEELEMENT_H
