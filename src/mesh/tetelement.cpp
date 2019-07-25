#include "tetelement.h"
#include "triangleface.h"

TetElement::TetElement()
  : Element("TetElement", 4, 6, 4)
{

}

TetElement::TetElement(int32_t number)
  : Element("TetElement", number, 4, 6, 4)
{

}

void TetElement::doComputeFeEntry(const Mesh * mesh, std::size_t localEdge1, std::size_t localEdge2, dcomplex & i1, dcomplex & i2) const
{

}

Face * TetElement::doConstructFace(std::size_t index) const
{
  TriangleFace * face = new TriangleFace();
  return face;
}

void TetElement::doEdgeNodes(std::size_t index, int32_t & localNode1, int32_t & localNode2) const
{

}
