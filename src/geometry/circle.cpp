#include "circle.h"

#include "xmlutils.h"

static const std::string OBJECT_ID("Circle");

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

std::vector<Segment> Circle::doGetSegmentList() const
{
  std::vector<Segment> segmentList;
  return segmentList;
}

std::vector<Vertex> Circle::doGetVertexList() const
{
  std::vector<Vertex> vertexList;
  return vertexList;
}

void Circle::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, "Circle");
  xmlutils::printValue(output, tabPos+2, "Name: ", name());
  xmlutils::printValue(output, tabPos+2, "Number: ", number());
  xmlutils::printValue(output, tabPos+2, "Center: ", mCenter);
  xmlutils::printValue(output, tabPos+2, "Radius: ", mRadius);
}

void Circle::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  setName(xmlutils::readAttribute<std::string>(node, "name"));
  setNumber(std::stoi(xmlutils::readAttribute<std::string>(node, "number")));
  setRadius(std::stod(xmlutils::readAttribute<std::string>(node, "radius")));
  mCenter.setX(std::stod(xmlutils::readAttribute<std::string>(node, "x")));
  mCenter.setY(std::stod(xmlutils::readAttribute<std::string>(node, "y")));
}

void Circle::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "name", name());
  xmlutils::writeAttribute(document, node, "number", number());
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, node, "x", mCenter.x());
  xmlutils::writeAttribute(document, node, "y", mCenter.y());
  xmlutils::writeAttribute(document, node, "radius", mRadius);
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

namespace  {
  Shape * createFunc()
  {
    return new Circle;
  }

  const bool registered = Shape::Factory::Instance().Register(OBJECT_ID, createFunc);
}
