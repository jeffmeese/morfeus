#include "plane.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

namespace Morfeus {
namespace Geometry {

static const std::string OBJECT_ID("Plane");

Plane::Plane()
  : Shape(OBJECT_ID)
{
  mPoint3 = computePoint3();
}

Plane::Plane(const Point3D & origin, const Point3D & point1, const Point3D & point2)
  : Shape(OBJECT_ID)
{
  mOrigin = origin;
  mPoint1 = point1;
  mPoint2 = point2;
  mPoint3 = computePoint3();
}

Plane::Plane(const std::string & name)
  : Shape("Plane", name)
{
  mPoint3 = computePoint3();
}

Plane::Plane(const std::string & name, const Point3D & origin, const Point3D & point1, const Point3D & point2)
  : Shape("Plane", name)
{
  mOrigin = origin;
  mPoint1 = point1;
  mPoint2 = point2;
  mPoint3 = computePoint3();
}

Point3D Plane::computePoint3() const
{
  double x = mPoint1.x() - mOrigin.x() + mPoint2.x();
  double y = mPoint1.y() - mOrigin.y() + mPoint2.y();
  double z = mPoint1.z() - mOrigin.z() + mPoint2.z();
  return Point3D(x, y, z);
}

std::vector<Face> Plane::doGetFacetList() const
{
  std::vector<Face> facets;

  Segment s1(0, 1);
  Segment s2(1, 2);
  Segment s3(2, 3);
  Segment s4(3, 0);

  Face facet;
  facet.addSegment(s1);
  facet.addSegment(s2);
  facet.addSegment(s3);
  facet.addSegment(s4);
  facet.setMarker(-1);
  facets.push_back(facet);

  return facets;
}

std::vector<Segment> Plane::doGetSegmentList() const
{
  std::vector<Segment> segments;
  return segments;
}

std::vector<Vertex> Plane::doGetVertexList() const
{
  std::vector<Vertex> vertexList;

  Vertex v1(0, mOrigin.x(), mOrigin.y(), mOrigin.z());
  Vertex v2(1, mPoint1.x(), mPoint1.y(), mPoint1.z());
  Vertex v3(2, mPoint3.x(), mPoint3.y(), mPoint3.z());
  Vertex v4(3, mPoint2.x(), mPoint2.y(), mPoint2.z());

  vertexList.push_back(v1);
  vertexList.push_back(v2);
  vertexList.push_back(v3);
  vertexList.push_back(v4);

  return vertexList;
}

void Plane::doPrint(std::ostream & output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
  xmlutils::printValue(output, tabPos, "x0: ", mOrigin.x());
  xmlutils::printValue(output, tabPos, "y0: ", mOrigin.y());
  xmlutils::printValue(output, tabPos, "z0: ", mOrigin.z());
  xmlutils::printValue(output, tabPos, "x1: ", mPoint1.x());
  xmlutils::printValue(output, tabPos, "y1: ", mPoint1.y());
  xmlutils::printValue(output, tabPos, "z1: ", mPoint1.z());
  xmlutils::printValue(output, tabPos, "x2: ", mPoint2.x());
  xmlutils::printValue(output, tabPos, "y2: ", mPoint2.y());
  xmlutils::printValue(output, tabPos, "z2: ", mPoint2.z());
}

void Plane::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  rapidxml::xml_node<> * centerNode = node->first_node("Origin", 0, false);
  if (centerNode != nullptr) {
    mOrigin.readFromXml(document, centerNode);
  }

  rapidxml::xml_node<> * point1Node = node->first_node("Point1", 0, false);
  if (point1Node != nullptr) {
    mPoint1.readFromXml(document, point1Node);
  }

  rapidxml::xml_node<> * point2Node = node->first_node("Point2", 0, false);
  if (point2Node != nullptr) {
    mPoint2.readFromXml(document, point2Node);
  }
}

void Plane::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  rapidxml::xml_node<> * centerNode = xmlutils::createNode(document, "Origin");
  mOrigin.writeToXml(document, centerNode);

  rapidxml::xml_node<> * point1Node = xmlutils::createNode(document, "Point1");
  mPoint1.writeToXml(document, point1Node);

  rapidxml::xml_node<> * point2Node = xmlutils::createNode(document, "Point2");
  mPoint2.writeToXml(document, point2Node);
}

namespace  {
  const bool r = Shape::factory().registerType(OBJECT_ID, boost::bind(boost::factory<Plane*>()));
}

}
}
