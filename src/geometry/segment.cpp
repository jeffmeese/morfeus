#include "segment.h"

Segment::Segment(int32_t id)
  : mId(id)
{
  mNode1 = mNode2 = -1;
  setBoundary(false);
}

Segment::Segment(int32_t id, int32_t node1, int32_t node2)
  : mId(id)
  , mNode1(node1)
  , mNode2(node2)
{
  setBoundary(false);
}

bool Segment::boundary() const
{
  return mBoundary;
}

int32_t Segment::id() const
{
  return mId;
}

int32_t Segment::node1() const
{
  return mNode1;
}

int32_t Segment::node2() const
{
  return mNode2;
}

void Segment::setBoundary(bool value)
{
  mBoundary = value;
}

void Segment::setNode1(int32_t node1)
{
  mNode1 = node1;
}

void Segment::setNode2(int32_t node2)
{
  mNode2 = node2;
}

void Segment::setNodes(int32_t node1, int32_t node2)
{
  setNode1(node1);
  setNode2(node2);
}
