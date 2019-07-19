#include "vertex.h"

Vertex::Vertex()
{
  setPosition(0.0, 0.0);
  setBoundary(false);
}

Vertex::Vertex(double x, double y)
{
  setPosition(x, y);
  setBoundary(false);
}

void Vertex::addAttribute(double value)
{
  mAttributes.push_back(value);
}

bool Vertex::boundary() const
{
  return mBoundary;
}

void Vertex::doReadFromXml(ptree &tree)
{

}

void Vertex::doWriteToXml(ptree &tree) const
{
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

double Vertex::x() const
{
  return mX;
}

double Vertex::y() const
{
  return mY;
}
