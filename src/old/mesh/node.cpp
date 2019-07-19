#include "mesh/node.h"

Node::Node(std::size_t id)
  : mId(id)
{
  move(0.0, 0.0, 0.0);
}

Node::Node(std::size_t id, double x, double y, double z)
  : mId(id)
{
  move(x, y, z);
}

std::size_t Node::id() const
{
  return mId;
}

void Node::move(double x, double y, double z)
{
  mX = x;
  mY = y;
  mZ = z;
}

void Node::setX(double x)
{
  mX = x;
}

void Node::setY(double y)
{
  mY = y;
}

void Node::setZ(double z)
{
  mZ = z;
}

void Node::scale(double sx, double sy, double sz)
{
  mX *= sx;
  mY *= sy;
  mZ *= sz;
}

void Node::translate(double dx, double dy, double dz)
{
  mX += dx;
  mY += dy;
  mZ += dz;
}

double Node::x() const
{
  return mX;
}

double Node::y() const
{
  return mY;
}

double Node::z() const
{
  return mZ;
}
