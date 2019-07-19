#include "mesh/element.h"

Element::Element(std::size_t id, std::size_t totalNodes, std::size_t totalEdges, std::size_t totalFaces)
  : mId(id)
  , mSection(0)
  , mEpsId(0)
  , mMuId(0)
  , mTotalFaces(totalFaces)
  , mType(Type::Dielectric)
{
  mGlobalEdges.resize(totalEdges, 0);
  mGlobalNodes.resize(totalNodes, 0);
  mEdgeSigns.resize(totalEdges, 1);
}

int Element::edgeSign(size_t localEdge) const
{
  return mEdgeSigns.at(localEdge);
}

std::size_t Element::epsId() const
{
  return mEpsId;
}

std::size_t Element::globalEdge(std::size_t localEdge) const
{
  return mGlobalEdges.at(localEdge);
}

std::size_t Element::globalNode(std::size_t localNode) const
{
  return mGlobalNodes.at(localNode);
}

std::size_t Element::id() const
{
  return mId;
}

std::size_t Element::muId() const
{
  return mMuId;
}

std::size_t Element::section() const
{
  return mSection;
}

void Element::setEdgeSign(size_t localEdge, int edgeSign)
{
  mEdgeSigns[localEdge] = edgeSign;
}

void Element::setEpsId(size_t epsId)
{
  mEpsId = epsId;
}

void Element::setGlobalEdge(std::size_t localEdge, std::size_t globalEdge)
{
  mGlobalEdges[localEdge] = globalEdge;
}

void Element::setGlobalNode(std::size_t localNode, std::size_t globalNode)
{
  mGlobalNodes[localNode] = globalNode;
}

void Element::setMuId(size_t muId)
{
  mMuId = muId;
}

void Element::setSection(size_t section)
{
  mSection = section;
}

void Element::setType(Type type)
{
  mType = type;
}

std::size_t Element::totalEdges() const
{
  return mGlobalEdges.size();
}

std::size_t Element::totalFaces() const
{
  return mTotalFaces;
}

std::size_t Element::totalNodes() const
{
  return mGlobalNodes.size();
}

Element::Type Element::type() const
{
  return mType;
}
