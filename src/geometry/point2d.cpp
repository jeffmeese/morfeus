#include "point2d.h"

Point2D::Point2D()
{
  setPosition(0.0, 0.0);
}

Point2D::Point2D(double x, double y)
{
  setPosition(x, y);
}

void Point2D::move(double dx, double dy)
{
  mX += dx;
  mY += dy;
}

void Point2D::setPosition(double x, double y)
{
  mX = x;
  mY = y;
}

void Point2D::setX(double value)
{
  mX = value;
}

void Point2D::setY(double value)
{
  mY = value;
}

double Point2D::x() const
{
  return mX;
}

double Point2D::y() const
{
  return mY;
}
