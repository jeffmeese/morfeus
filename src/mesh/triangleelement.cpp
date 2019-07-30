#include "triangleelement.h"
#include "triangleface.h"

namespace morfeus {
namespace mesh {

TriangleElement::TriangleElement()
  : Element("TriangleElement", 3, 3, 1)
{
}

TriangleElement::TriangleElement(int32_t number)
  : Element("TriangleElement", number, 3, 3, 1)
{
}

void TriangleElement::doComputeFeEntry(const Mesh * mesh, std::size_t localEdge1, std::size_t localEdge2, dcomplex & i1, dcomplex & i2) const
{
}

Face * TriangleElement::doConstructFace(std::size_t) const
{
  Face * face = new TriangleFace;
  face->setNode(0, node(0));
  face->setNode(1, node(1));
  face->setNode(2, node(2));
  face->setEdge(0, edge(0));
  face->setEdge(1, edge(1));
  face->setEdge(2, edge(2));
  return face;
}

void TriangleElement::doEdgeNodes(std::size_t index, int32_t & localNode1, int32_t & localNode2) const
{
  static const int edge_node1[] = {0, 1, 2};
  static const int edge_node2[] = {1, 2, 0};
  localNode1 = edge_node1[index];
  localNode2 = edge_node2[index];
}

}
}
