#include "triangleelement.h"
#include "triangleface.h"

TriangleElement::TriangleElement(int32_t id)
  : Element(id, 3, 3, 1)
{
}

void TriangleElement::doEdgeNodes(int32_t index, int32_t & localNode1, int32_t & localNode2) const
{
  static const int edge_node1[] = {1, 2, 3};
  static const int edge_node2[] = {2, 3, 1};
  localNode1 = edge_node1[index];
  localNode2 = edge_node2[index];
}

dcomplex TriangleElement::doComputeFeEntry(const Mesh & mesh, int32_t localEdge1, int32_t localEdge2) const
{
  dcomplex result;
  return result;
}

Face * TriangleElement::doConstructFace(int32_t) const
{
  Face * face = new TriangleFace(id());
  face->setNode(1, node(1));
  face->setNode(2, node(2));
  face->setNode(3, node(3));
  face->setEdge(1, edge(1));
  face->setEdge(2, edge(2));
  face->setEdge(3, edge(3));
  return face;
}

