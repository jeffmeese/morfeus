#include "hole.h"
#include "geometry.h"

Hole::Hole()
{
  setPosition(0.0, 0.0);
}

Hole::Hole( double x, double y)
{
  setPosition(x, y);
}

void Hole::doAddToGeometry(Geometry *geometry) const
{
//  std::size_t totalHoles = geometry->totalShapes();

//  int32_t id = static_cast<int32_t>(totalHoles);
//  std::unique_ptr<Hole> hole(new Hole(id));
//  hole->setPosition(mX, mY);
//  hole->setName(name());
//  geometry->addShape(std::move(hole));
}

void Hole::doReadFromXml(ptree &tree)
{

}

void Hole::doWriteToXml(ptree &tree) const
{
}

double Hole::x() const
{
  return mX;
}

double Hole::y() const
{
  return mY;
}

void Hole::setPosition(double x, double y)
{
  setX(x);
  setY(y);
}

void Hole::setX(double value)
{
  mX = value;
}

void Hole::setY(double value)
{
  mY = value;
}
