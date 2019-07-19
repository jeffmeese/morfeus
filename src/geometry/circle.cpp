#include "circle.h"

Circle::Circle()
  : mCenter(0.0, 0.0)
  , mRadius(1.0)
{
}

Circle::Circle(const Point2D & center, double radius)
  : mCenter(center)
  , mRadius(radius)
{
}

Circle::Circle(double x, double y, double radius)
  : mCenter(x, y)
  , mRadius(radius)
{
}

Point2D Circle::center() const
{
  return mCenter;
}

void Circle::doReadFromXml(ptree &tree)
{

}

void Circle::doWriteToXml(ptree &tree) const
{

}

double Circle::radius() const
{
  return mRadius;
}

void Circle::setCenter(const Point2D & value)
{
  mCenter.setPosition(value.x(), value.y());
}

void Circle::setCenter(double x, double y)
{
  mCenter.setPosition(x, y);
}

void Circle::setRadius(double value)
{
  mRadius = value;
}

