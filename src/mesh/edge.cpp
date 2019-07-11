#include "edge.h"

Edge::Edge(int32_t id)
  : mId(id)
{
  mNode1 = mNode2 = -1;
}

Edge::Edge(int32_t id, int32_t node1, int32_t node2)
  : mId(id)
  , mNode1(node1)
  , mNode2(node2)
{
}

int32_t Edge::id() const
{
  return mId;
}

int32_t Edge::node1() const
{
  return mNode1;
}

int32_t Edge::node2() const
{
  return mNode2;
}

void Edge::setNode1(int32_t value)
{
  mNode1 = value;
}

void Edge::setNode2(int32_t value)
{
  mNode2 = value;
}
