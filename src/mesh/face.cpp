#include "face.h"

#include "mesh.h"
#include "node.h"

#include <algorithm>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/geometry/arithmetic/cross_product.hpp>

Face::Face(const std::string & type, int32_t number, std::size_t totalNodes, std::size_t totalEdges)
  : MorfeusObject(type)
  , mNumber(number)
{
  init(totalNodes, totalEdges);
}

Face::Face(const std::string & type, std::size_t totalNodes, std::size_t totalEdges)
  : MorfeusObject(type)
  , mNumber(-1)
{
  init(totalNodes, totalEdges);
}

double Face::computeArea(const Mesh * mesh) const
{
  return doComputeArea(mesh);
}

double Face::computeNormal(const Mesh * mesh) const
{
//  boost::numeric::ublas::vector<double> v1(3);
//  boost::numeric::ublas::vector<double> v2(3);
//  boost::numeric::ublas::vector<double> v3(3);

//  for (std::size_t i = 0; i < totalNodes(); i++) {
//    const Node * currNode = mesh->node(mNodes[i]);
//    v1[i] = currNode->x();
//    v2[i] = currNode->y();
//    v3[i] = currNode->z();
//  }

//  boost::numeric::ublas::vector<double> v4(v1-v2);
//  boost::numeric::ublas::vector<double> v5(v1-v3);
//  boost::numeric::ublas::vector<double> cp = boost::geometry::cross_product(v4, v5);

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

void Face::init(std::size_t totalNodes, std::size_t totalEdges)
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
