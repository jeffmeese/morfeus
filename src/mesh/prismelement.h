#ifndef PRISMELEMENT_H
#define PRISMELEMENT_H

#include "morfeus.h"
#include "element.h"

class PrismElement
    : public Element
{
public:
  MORFEUS_LIB_DECL PrismElement(int32_t id);

protected:
  dcomplex doComputeFeEntry(const Mesh & mesh, int32_t localEdge1, int32_t localEdge2) const override;
  void doEdgeNodes(int32_t index, int32_t & localNode1, int32_t & localNode2) const override;
  Face * doConstructFace(int32_t index) const override;
};

#endif // PRISMELEMENT_H
