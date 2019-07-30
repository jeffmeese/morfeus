#include "cylinder.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

namespace morfeus {
namespace geometry {

static const std::string OBJECT_ID("Cylinder");

Cylinder::Cylinder()
  : Shape(OBJECT_ID)
{
  setCenter(Point3D(0.0,0.0,0.0));
  setRadius(0.5);
  setHeight(1.0);
  setResolution(6);
}

Cylinder::Cylinder(const Point3D & center, double radius, double height)
  : Shape(OBJECT_ID)
{
  setCenter(center);
  setRadius(radius);
  setHeight(height);
  setResolution(6);
}

Cylinder::Cylinder(const std::string & name)
  : Shape(OBJECT_ID, name)
{
  setCenter(Point3D(0.0,0.0,0.0));
  setRadius(0.5);
  setHeight(1.0);
  setResolution(6);
}

Cylinder::Cylinder(const std::string & name, const Point3D & center, double radius, double height)
  : Shape(OBJECT_ID, name)
{
  setCenter(center);
  setRadius(radius);
  setHeight(height);
  setResolution(6);
}

std::vector<Face> Cylinder::doGetFacetList() const
{
  std::vector<Face> facets;
  return facets;
}

std::vector<Segment> Cylinder::doGetSegmentList() const
{
  std::vector<Segment> segments;
  return segments;
}

std::vector<Vertex> Cylinder::doGetVertexList() const
{
  std::vector<Vertex> vertexList;
  return vertexList;
}

void Cylinder::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
  xmlutils::printValue(output, tabPos, "x: ", mCenter.x());
  xmlutils::printValue(output, tabPos, "y: ", mCenter.y());
  xmlutils::printValue(output, tabPos, "z: ", mCenter.z());
  xmlutils::printValue(output, tabPos, "radius: ", mRadius);
  xmlutils::printValue(output, tabPos, "height: ", mHeight);
  xmlutils::printValue(output, tabPos, "resolution: ", mResolution);
}

void Cylinder::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  double x = xmlutils::readAttribute<double>(node, "x");
  double y = xmlutils::readAttribute<double>(node, "y");
  double z = xmlutils::readAttribute<double>(node, "z");

  setCenter(Point3D(x, y, z));
  setRadius(xmlutils::readAttribute<double>(node, "radius"));
  setHeight(xmlutils::readAttribute<double>(node, "height"));
  setResolution(xmlutils::readAttribute<std::size_t>(node, "resolution"));
}

void Cylinder::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, node, "radius", mRadius);
  xmlutils::writeAttribute(document, node, "height", mHeight);
  xmlutils::writeAttribute(document, node, "resolution", mResolution);
  rapidxml::xml_node<> * centerNode = xmlutils::createNode(document, "Center");
  mCenter.writeToXml(document, centerNode);
}

namespace  {
  const bool r = Shape::factory().registerType(OBJECT_ID, boost::bind(boost::factory<Cylinder*>()));
}

}
}
