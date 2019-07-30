#include "point2d.h"

namespace morfeus {
namespace geometry {

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

void Point2D::print(std::ostream & output, int tabPos) const
{
  xmlutils::printValue(output, tabPos, "x : ", mX);
  xmlutils::printValue(output, tabPos, "y : ", mY);
}

void Point2D::print(int tabPos) const
{
  print(std::cout, tabPos);
}

void Point2D::readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  setX(xmlutils::readAttribute<double>(node, "mX"));
  setX(xmlutils::readAttribute<double>(node, "mY"));
}

void Point2D::setPosition(double x, double y)
{
  mX = x;
  mY = y;
}

void Point2D::writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "x", mX);
  xmlutils::writeAttribute(document, node, "y", mY);
}

std::ostream & operator<<(std::ostream & output, const Point2D & point)
{
  output << "(" << point.x() << "," << point.y();
  return output;
}

}
}
