#include "point.h"

namespace morfeus {
namespace geometry {

Point::Point()
  : core::MorfeusObject ("Point")
{
  setPosition(0.0, 0.0, 0.0);
}

Point::Point(double x, double y, double z)
  : core::MorfeusObject ("Point")
{
  setPosition(x, y, z);
}

void Point::doPrint(std::ostream & output, int tabPos) const
{
  xmlutils::printValue(output, tabPos, "x : ", mX);
  xmlutils::printValue(output, tabPos, "y : ", mY);
  xmlutils::printValue(output, tabPos, "z : ", mZ);
}

void Point::doXmlRead(rapidxml::xml_document<> &document, rapidxml::xml_node<> *node)
{
  setX(xmlutils::readAttribute<double>(node, "x"));
  setY(xmlutils::readAttribute<double>(node, "y"));
  setZ(xmlutils::readAttribute<double>(node, "z"));
}


void Point::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "x", mX);
  xmlutils::writeAttribute(document, node, "y", mY);
  xmlutils::writeAttribute(document, node, "z", mZ);
}

void Point::move(double dx, double dy, double dz)
{
  mX += dx;
  mY += dy;
  mZ += dz;
}

void Point::setPosition(double x, double y, double z)
{
  mX = x;
  mY = y;
  mZ = z;
}

std::ostream & operator<<(std::ostream & output, const Point & point)
{
  output << "(" << point.x() << "," << point.y() << "," << point.z();
  return output;
}

}
}
