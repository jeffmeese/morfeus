#include "face.h"

#include "mesh.h"
#include "node.h"

#include <algorithm>

Face::Face(int32_t id, std::size_t totalNodes, std::size_t totalEdges)
  : mId(id)
  , mPosition(Position::Top)
{
  mNodes.resize(totalNodes);
  mEdges.resize(totalEdges);
  mEdgeSigns.resize(totalEdges);

  for (std::size_t i = 0; i < totalNodes; i++) {
    mNodes[i] = -1;
  }
  for (std::size_t i = 0; i < totalEdges; i++) {
    mEdges[i] = -1;
    mEdgeSigns[i] = 1;
  }
}

Face::~Face()
{
}

double Face::computeArea(const Mesh * mesh) const
{
  return doComputeArea(mesh);
}

double Face::computeNormal(const Mesh * mesh) const
{
  return 0.0;
//  math::vector<double> normalVector(0.0, 0.0, 0.0);

//  for (std::size_t i = 0; i < totalNodes()-1; i++) {
//    std::size_t currentNodeIndex = mGlobalNodes[i];
//    std::size_t nextNodeIndex = mGlobalNodes[i+1];
//    Node * currentNode = mesh->getNode(currentNodeIndex);
//    Node * nextNode = mesh->getNode(nextNodeIndex);

//    double tx = (currentNode->y()-nextNode->y()) - (currentNode->z()+nextNode->z());
//    double ty = (currentNode->z()-nextNode->z()) - (currentNode->x()+nextNode->x());
//    double tz = (currentNode->x()-nextNode->x()) - (currentNode->y()+nextNode->y());

//    normalVector.translate(tx, ty, tz);
//  }

//  normalVector.normalize();
//  return normalVector;
}

dcomplex Face::computeMomEntry(const Face * otherFace, const Mesh * mesh, std::size_t localEdge) const
{
  return doComputeMomEntry(otherFace, mesh, localEdge);
}

dcomplex Face::computePlanewaveEntry(std::size_t edge, double freq, const Planewave *planewave, const Mesh * mesh) const
{
  return doComputePlanewaveEntry(edge, freq, planewave, mesh);
}

int32_t Face::edgeSign(std::size_t localEdge) const
{
  return mEdgeSigns.at(localEdge);
}

int32_t Face::id() const
{
  return mId;
}

bool Face::isCoincident(const Face *otherFace) const
{
  // Make sure both faces have the same number of nodes
  if (totalNodes() != otherFace->totalNodes())
    return false;

  // Determine if both faces contain the same global node numbers
  using iterator = std::vector<int32_t>::const_iterator;
  for (std::size_t i = 0; i < mNodes.size(); i++) {
    int32_t globalNode = mNodes.at(i);
    iterator itr = std::find(otherFace->mNodes.begin(), otherFace->mNodes.end(), globalNode);
    if (itr == otherFace->mNodes.end())
      return false;
  }

  return true;
}

int32_t Face::edge(std::size_t index) const
{
  return mEdges[index];
}

int32_t Face::node(std::size_t index) const
{
  return mNodes[index];
}

Face::Position Face::position() const
{
  return mPosition;
}

void Face::setEdge(std::size_t index, int32_t value)
{
  mEdges[index] = value;
}

void Face::setEdgeSign(std::size_t localEdge, int32_t edgeSign)
{
  mEdgeSigns[localEdge] = edgeSign;
}

void Face::setNode(std::size_t index, int32_t value)
{
  mNodes[index] = value;
}

void Face::setPosition(Position position)
{
  mPosition = position;
}

std::size_t Face::totalEdges() const
{
  return mEdges.size();
}

std::size_t Face::totalNodes() const
{
  return mNodes.size();
}
