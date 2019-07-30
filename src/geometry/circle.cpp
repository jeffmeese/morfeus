#include "circle.h"

#include "xmlutils.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

namespace Morfeus {
namespace Geometry {

static const std::string OBJECT_ID("Circle");

Circle::Circle()
  : Shape(OBJECT_ID)
{
  setCenter(Point3D(0.0, 0.0, 0.0));
  setRadius(1.0);
  setResolution(6);
}

Circle::Circle(const std::string & name)
  : Shape(OBJECT_ID, name)
{
  setCenter(Point3D(0.0, 0.0, 0.0));
  setRadius(1.0);
  setResolution(6);
}

Circle::Circle(const std::string & id, const std::string & name)
  : Shape(OBJECT_ID, id, name)
{
  setCenter(Point3D(0.0, 0.0, 0.0));
  setRadius(1.0);
  setResolution(6);
}

Circle::Circle(const Point3D & center, double radius)
  : Shape(OBJECT_ID)
{
  setCenter(center);
  setRadius(radius);
  setResolution(6);
}

Circle::Circle(const std::string & name, const Point3D & center, double radius)
  : Shape(OBJECT_ID, name)
{
  setCenter(center);
  setRadius(radius);
  setResolution(6);
}

Circle::Circle(const std::string & id, const std::string & name, const Point3D & center, double radius)
  : Shape(OBJECT_ID, id, name)
{
  setCenter(center);
  setRadius(radius);
  setResolution(6);
}

std::vector<Face> Circle::doGetFacetList() const
{
  std::vector<Face> facets;
  return facets;
}

std::vector<Segment> Circle::doGetSegmentList() const
{
  std::vector<Segment> segments;
  return segments;
}


std::vector<Vertex> Circle::doGetVertexList() const
{
  std::vector<Vertex> vertexList;
  return vertexList;
}

void Circle::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
  xmlutils::printValue(output, tabPos, "x: ", mCenter.x());
  xmlutils::printValue(output, tabPos, "y: ", mCenter.y());
  xmlutils::printValue(output, tabPos, "z: ", mCenter.z());
  xmlutils::printValue(output, tabPos, "Radius: ", mRadius);
  xmlutils::printValue(output, tabPos, "Resolution: ", mResolution);
}

void Circle::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  setRadius(xmlutils::readAttribute<double>(node, "radius"));
  setResolution(xmlutils::readAttribute<std::size_t>(node, "resolution"));
  rapidxml::xml_node<> * centerNode = node->first_node("Center", 0, false);
  if (centerNode != nullptr) {
    mCenter.readFromXml(document, centerNode);
  }
}

void Circle::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, node, "radius", mRadius);
  xmlutils::writeAttribute(document, node, "resolution", mResolution);
  rapidxml::xml_node<> * centerNode = xmlutils::createNode(document, "Center");
  mCenter.writeToXml(document, centerNode);
}

namespace  {
  const bool r = Shape::factory().registerType(OBJECT_ID, boost::bind(boost::factory<Circle*>()));
}

}
}
