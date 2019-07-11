#include "prismelement.h"

PrismElement::PrismElement(int32_t id)
  : Element(id, 6, 9, 5)
{
}

dcomplex PrismElement::doComputeFeEntry(const Mesh & mesh, int32_t localEdge1, int32_t localEdge2) const
{
  dcomplex result;
  return result;
}

void PrismElement::doEdgeNodes(int32_t index, int32_t & localNode1, int32_t & localNode2) const
{
  static const int edge_node1[] = {1, 2, 3, 4, 5, 6, 1, 2, 3};
  static const int edge_node2[] = {2, 3, 1, 5, 6, 4, 7, 8, 9};
  localNode1 = edge_node1[index];
  localNode2 = edge_node2[index];
}

Face * PrismElement::doConstructFace(int32_t index) const
{
  return nullptr;
}
