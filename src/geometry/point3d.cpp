#include "point3d.h"

Point3D::Point3D()
{
  setPosition(0.0, 0.0, 0.0);
}

Point3D::Point3D(double x, double y, double z)
{
  setPosition(x, y, z);
}

void Point3D::move(double dx, double dy, double dz)
{
  mX += dx;
  mY += dy;
  mZ += dz;
}

void Point3D::setPosition(double x, double y, double z)
{
  mX = x;
  mY = y;
  mZ = z;
}

void Point3D::setX(double value)
{
  mX = value;
}

void Point3D::setY(double value)
{
  mY = value;
}

void Point3D::setZ(double value)
{
  mZ = value;
}

double Point3D::x() const
{
  return mX;
}

double Point3D::y() const
{
  return mY;
}

double Point3D::z() const
{
  return mZ;
}

std::ostream & operator<<(std::ostream & output, const Point3D & point)
{
  output << "(" << point.x() << "," << point.y() << "," << point.z();
  return output;
}
