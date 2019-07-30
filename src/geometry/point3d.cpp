#include "point3d.h"

namespace morfeus {
namespace geometry {

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

void Point3D::print(std::ostream & output, int tabPos) const
{
  xmlutils::printValue(output, tabPos, "x : ", mX);
  xmlutils::printValue(output, tabPos, "y : ", mY);
  xmlutils::printValue(output, tabPos, "z : ", mZ);
}

void Point3D::print(int tabPos) const
{
  print(std::cout, tabPos);
}

void Point3D::readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  setX(xmlutils::readAttribute<double>(node, "x"));
  setY(xmlutils::readAttribute<double>(node, "y"));
  setZ(xmlutils::readAttribute<double>(node, "z"));
}

void Point3D::setPosition(double x, double y, double z)
{
  mX = x;
  mY = y;
  mZ = z;
}

void Point3D::writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "x", mX);
  xmlutils::writeAttribute(document, node, "y", mY);
  xmlutils::writeAttribute(document, node, "z", mZ);
}

std::ostream & operator<<(std::ostream & output, const Point3D & point)
{
  output << "(" << point.x() << "," << point.y() << "," << point.z();
  return output;
}

}
}
