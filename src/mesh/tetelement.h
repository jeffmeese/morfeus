#ifndef TETELEMENT_H
#define TETELEMENT_H

#include "element.h"

namespace Morfeus {
namespace mesh {

class TetElement
    : public Element
{
public:
  MORFEUS_LIB_DECL TetElement();
  MORFEUS_LIB_DECL TetElement(int32_t number);

protected:
  void doComputeFeEntry(const Mesh * mesh, std::size_t localEdge1, std::size_t localEdge2, dcomplex & i1, dcomplex & i2) const override;
  Face * doConstructFace(std::size_t index) const override;
  void doEdgeNodes(std::size_t index, int32_t & localNode1, int32_t & localNode2) const override;
};

}
}

#endif // TETELEMENT_H
