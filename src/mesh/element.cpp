#include "element.h"

Element::Element(int32_t id, int32_t nodes, int32_t edges, int32_t faces)
  : mId(id)
  , mFaces(faces)
{
  mEdges.resize(static_cast<std::size_t>(edges));
  mNodes.resize(static_cast<std::size_t>(nodes));
  for (std::size_t i = 0; i < static_cast<std::size_t>(edges); i++) {
    mEdges[i] = -1;
  }
  for (std::size_t i = 0; i < static_cast<std::size_t>(nodes); i++) {
    mNodes[i] = -1;
  }
}

Element::~Element()
{
}

void Element::addAttribute(double value)
{
  mAttributes.push_back(value);
}

double Element::attribute(int32_t index) const
{
  return mAttributes[static_cast<std::size_t>(index)];
}

int32_t Element::edge(int32_t index) const
{
  return mEdges[std::size_t(index-1)];
}

void Element::edgeNodes(int32_t index, int32_t &node1, int32_t &node2) const
{
  doEdgeNodes(index, node1, node2);
}

Face * Element::constructFace(int32_t index) const
{
  return doConstructFace(index);
}

int32_t Element::id() const
{
  return mId;
}

int32_t Element::node(int32_t index) const
{
  return mNodes[std::size_t(index-1)];
}

void Element::setEdge(int32_t index, int32_t value)
{
  mEdges[std::size_t(index-1)] = value;
}

void Element::setNode(int32_t index, int32_t value)
{
  mNodes[std::size_t(index-1)] = value;
}

int32_t Element::totalAttributes() const
{
  return static_cast<int32_t>(mAttributes.size());
}

int32_t Element::totalEdges() const
{
  return static_cast<int32_t>(mEdges.size());
}

int32_t Element::totalFaces() const
{
  return mFaces;
}

int32_t Element::totalNodes() const
{
  return static_cast<int32_t>(mNodes.size());
}

