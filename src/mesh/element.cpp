#include "element.h"

namespace morfeus {
namespace mesh {

Element::Element(const std::string & type, std::size_t nodes, std::size_t edges, std::size_t faces)
  : MorfeusObject(type)
  , mAttribute(DielectricElement)
  , mNumber(-1)
{
  init(nodes, edges, faces);
}

Element::Element(const std::string & type, int32_t number, std::size_t nodes, std::size_t edges, std::size_t faces)
  : MorfeusObject(type)
  , mAttribute(DielectricElement)
  , mNumber(number)
{
  init(nodes, edges, faces);
}

Face * Element::constructFace(std::size_t index) const
{
  return doConstructFace(index);
}

void Element::computeFeEntry(const Mesh * mesh, std::size_t localEdge1, std::size_t localEdge2, dcomplex & i1, dcomplex & i2) const
{
  doComputeFeEntry(mesh, localEdge1, localEdge2, i1, i2);
}

void Element::edgeNodes(std::size_t index, int32_t &node1, int32_t &node2) const
{
  doEdgeNodes(index, node1, node2);
}

void Element::init(std::size_t nodes, std::size_t edges, std::size_t faces)
{
  mEpsilonId = mMuId = -1;
  mFaces = faces;
  mEdges.resize(edges);
  mNodes.resize(nodes);
  mEdgeSigns.resize(edges);
  for (std::size_t i = 0; i < edges; i++) {
    mEdges[i] = -1;
    mEdgeSigns[i] = 1;
  }
  for (std::size_t i = 0; i < nodes; i++) {
    mNodes[i] = -1;
  }
}

}
}
