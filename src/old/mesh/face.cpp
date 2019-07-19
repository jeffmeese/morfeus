#include "mesh/face.h"
#include "mesh/mesh.h"
#include "mesh/node.h"

#include <algorithm>
#include <numeric>

Face::Face(std::size_t nodes, std::size_t edges)
  : mPosition(Position::Top)
  , mSectionId(0)
{
  mGlobalNodes.resize(nodes, 0);
  mGlobalEdges.resize(edges, 0);
  mEdgeSigns.resize(edges, 1);
}

math::vector<double> Face::computeNormal(const Mesh & mesh) const
{
  math::vector<double> normalVector(0.0, 0.0, 0.0);

  for (std::size_t i = 0; i < totalNodes()-1; i++) {
    std::size_t currentNodeIndex = mGlobalNodes[i];
    std::size_t nextNodeIndex = mGlobalNodes[i+1];
    Node * currentNode = mesh.getNode(currentNodeIndex);
    Node * nextNode = mesh.getNode(nextNodeIndex);

    double tx = (currentNode->y()-nextNode->y()) - (currentNode->z()+nextNode->z());
    double ty = (currentNode->z()-nextNode->z()) - (currentNode->x()+nextNode->x());
    double tz = (currentNode->x()-nextNode->x()) - (currentNode->y()+nextNode->y());

    normalVector.translate(tx, ty, tz);
  }

  normalVector.normalize();
  return normalVector;
}

int Face::edgeSign(size_t localEdge) const
{
  return mEdgeSigns.at(localEdge);
}

std::size_t Face::globalEdge(std::size_t localEdge) const
{
  return mGlobalEdges.at(localEdge);
}

std::size_t Face::globalNode(std::size_t localNode) const
{
  return mGlobalNodes.at(localNode);
}

bool Face::isCoincident(const Face *otherFace) const
{
  // Make sure both faces have the same number of nodes
  if (totalNodes() != otherFace->totalNodes())
    return false;

  // Determine if both faces contain the same global node numbers
  using iterator = std::vector<std::size_t>::const_iterator;
  for (std::size_t i = 0; i < mGlobalNodes.size(); i++) {
    std::size_t globalNode = mGlobalNodes.at(i);
    iterator itr = std::find(otherFace->mGlobalNodes.begin(), otherFace->mGlobalNodes.end(), globalNode);
    if (itr == otherFace->mGlobalNodes.end())
      return false;
  }

  return true;
}

Face::Position Face::position() const
{
  return mPosition;
}

std::size_t Face::sectionId() const
{
  return mSectionId;
}

void Face::setEdgeSign(size_t localEdge, int edgeSign)
{
  mEdgeSigns[localEdge] = edgeSign;
}

void Face::setGlobalEdge(std::size_t localEdge, std::size_t globalEdge)
{
  mGlobalEdges[localEdge] = globalEdge;
}

void Face::setGlobalNode(std::size_t localNode, std::size_t globalNode)
{
  mGlobalNodes[localNode] = globalNode;
}

void Face::setPosition(Position position)
{
  mPosition = position;
}

void Face::setSectionId(std::size_t id)
{
  mSectionId = id;
}

std::size_t Face::totalEdges() const
{
  return mGlobalEdges.size();
}

std::size_t Face::totalNodes() const
{
  return mGlobalNodes.size();
}
