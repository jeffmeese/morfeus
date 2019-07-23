#include "circle.h"

#include "xmlutils.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

static const std::string OBJECT_ID("Circle");

Circle::Circle()
  : Shape(OBJECT_ID)
  , mCenter(0.0, 0.0)
  , mRadius(1.0)
{
}

Circle::Circle(const std::string & name)
  : Shape(OBJECT_ID, name)
  , mCenter(0.0, 0.0)
  , mRadius(1.0)
{
}

Circle::Circle(const std::string & id, const std::string & name)
  : Shape(OBJECT_ID, id, name)
  , mCenter(0.0, 0.0)
  , mRadius(1.0)
{
}

Circle::Circle(double x, double y, double radius)
  : Shape(OBJECT_ID)
  , mCenter(x, y)
  , mRadius(radius)
{
}

Circle::Circle(const std::string & name, double x, double y, double radius)
  : Shape(OBJECT_ID, name)
  , mCenter(x, y)
  , mRadius(radius)
{
}

Circle::Circle(const std::string & id, const std::string & name, double x, double y, double radius)
  : Shape(OBJECT_ID, id, name)
  , mCenter(x, y)
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
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
  xmlutils::printValue(output, tabPos, "Center: ", mCenter);
  xmlutils::printValue(output, tabPos, "Radius: ", mRadius);
}

void Circle::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  setRadius(xmlutils::readAttribute<double>(node, "radius"));
  mCenter.setX(xmlutils::readAttribute<double>(node, "x"));
  mCenter.setY(xmlutils::readAttribute<double>(node, "y"));
}

void Circle::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
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
  const bool r = Shape::factory().registerType(OBJECT_ID, boost::bind(boost::factory<Circle*>()));
}
