#include "vertex.h"

Vertex::Vertex(int32_t id)
  : mId(id)
{
  setPosition(0.0, 0.0);
  setBoundary(false);
}

Vertex::Vertex(int32_t id, double x, double y)
  : mId(id)
{
  setPosition(x, y);
  setBoundary(false);
}

bool Vertex::boundary() const
{
  return mBoundary;
}

int32_t Vertex::id() const
{
  return mId;
}

double Vertex::x() const
{
  return mX;
}

double Vertex::y() const
{
  return mY;
}

void Vertex::setBoundary(bool value)
{
  mBoundary = value;
}

void Vertex::setPosition(double x, double y)
{
  setX(x);
  setY(y);
}

void Vertex::setX(double value)
{
  mX = value;
}

void Vertex::setY(double value)
{
  mY = value;
}

void Vertex::addAttribute(double value)
{
  mAttributes.push_back(value);
}
