#include "mesh/edge.h"
#include "mesh/node.h"
#include "mesh/mesh.h"

Edge::Edge(std::size_t id)
  : mId(id)
{
  setGlobalNode1(0);
  setGlobalNode2(0);
  setType(Type::Dielectric);
  setUnknown(0);
}

Edge::Edge(std::size_t id, std::size_t globalNode1, std::size_t globalNode2)
  : mId(id)
{
  setGlobalNode1(globalNode1);
  setGlobalNode2(globalNode2);
  setType(Type::Dielectric);
  setUnknown(0);
}

double Edge::computeLength(const Mesh &mesh) const
{
  Node * node1 = mesh.getNode(mNode1);
  Node * node2 = mesh.getNode(mNode2);

  double x1 = node1->x();
  double y1 = node1->y();
  double z1 = node1->z();

  double x2 = node2->x();
  double y2 = node2->y();
  double z2 = node2->z();

  double length = sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1) );
  return length;
}

std::size_t Edge::globalNode1() const
{
  return mNode1;
}

std::size_t Edge::globalNode2() const
{
  return mNode2;
}

std::size_t Edge::id() const
{
  return mId;
}

Edge::Type Edge::type() const
{
  return mType;
}

void Edge::setGlobalNode1(size_t globalNode)
{
  mNode1 = globalNode;
}

void Edge::setGlobalNode2(size_t globalNode)
{
  mNode2 = globalNode;
}

void Edge::setType(Type type)
{
  mType = type;
}

void Edge::setUnknown(int unknown)
{
  mUnknown = unknown;
}

int Edge::unknown() const
{
  return mUnknown;
}
