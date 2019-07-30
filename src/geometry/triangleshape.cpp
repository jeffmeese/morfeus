#include "triangleshape.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

namespace Morfeus {
namespace Geometry {

static const std::string OBJECT_ID("Triangle");

TriangleShape::TriangleShape()
  : Shape(OBJECT_ID)
{
  setPoint1(Point3D(0.0,0.0,0.0));
  setPoint2(Point3D(0.0,0.0,0.0));
  setPoint3(Point3D(0.0,0.0,0.0));
}

TriangleShape::TriangleShape(const std::string & name)
  : Shape (OBJECT_ID, name)
{
  setPoint1(Point3D(0.0,0.0,0.0));
  setPoint2(Point3D(0.0,0.0,0.0));
  setPoint3(Point3D(0.0,0.0,0.0));
}

TriangleShape::TriangleShape(const std::string & id, const std::string & name)
  : Shape(OBJECT_ID, id, name)
{
  setPoint1(Point3D(0.0,0.0,0.0));
  setPoint2(Point3D(0.0,0.0,0.0));
  setPoint3(Point3D(0.0,0.0,0.0));
}

TriangleShape::TriangleShape(const Point3D & pt1, const Point3D & pt2, const Point3D & pt3)
  : Shape(OBJECT_ID)
{
  setPoint1(pt1);
  setPoint2(pt2);
  setPoint3(pt3);
}

TriangleShape::TriangleShape(const std::string & name, const Point3D & pt1, const Point3D & pt2, const Point3D & pt3)
  : Shape (OBJECT_ID, name)
{
  setPoint1(pt1);
  setPoint2(pt2);
  setPoint3(pt3);
}

TriangleShape::TriangleShape(const std::string & id, const std::string & name, const Point3D & pt1, const Point3D & pt2, const Point3D & pt3)
  : Shape (OBJECT_ID, id, name)
{
  setPoint1(pt1);
  setPoint2(pt2);
  setPoint3(pt3);
}

std::vector<Face> TriangleShape::doGetFacetList() const
{
  std::vector<Face> facets;
  return facets;
}

std::vector<Segment> TriangleShape::doGetSegmentList() const
{
  std::vector<Segment> segments;
  return segments;
}

std::vector<Vertex> TriangleShape::doGetVertexList() const
{
  std::vector<Vertex> vertexList;
  return vertexList;
}

void TriangleShape::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
  xmlutils::printValue(output, tabPos, "x1", mPoint1.x());
  xmlutils::printValue(output, tabPos, "y1", mPoint1.y());
  xmlutils::printValue(output, tabPos, "z1", mPoint1.z());
  xmlutils::printValue(output, tabPos, "x2", mPoint2.x());
  xmlutils::printValue(output, tabPos, "y2", mPoint2.y());
  xmlutils::printValue(output, tabPos, "z2", mPoint2.z());
  xmlutils::printValue(output, tabPos, "x3", mPoint3.x());
  xmlutils::printValue(output, tabPos, "y3", mPoint3.y());
  xmlutils::printValue(output, tabPos, "z3", mPoint3.z());
}

void TriangleShape::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  rapidxml::xml_node<> * pt1Node = node->first_node("Point1", 0, false);
  if (pt1Node != nullptr) {
    mPoint1.readFromXml(document, pt1Node);
  }

  rapidxml::xml_node<> * pt2Node = node->first_node("Point2", 0, false);
  if (pt2Node != nullptr) {
    mPoint2.readFromXml(document, pt2Node);
  }

  rapidxml::xml_node<> * pt3Node = node->first_node("Point3", 0, false);
  if (pt3Node != nullptr) {
    mPoint3.readFromXml(document, pt3Node);
  }
}

void TriangleShape::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);

  rapidxml::xml_node<> * pt1Node = xmlutils::createNode(document, "Point1");
  mPoint1.writeToXml(document, pt1Node);

  rapidxml::xml_node<> * pt2Node = xmlutils::createNode(document, "Point2");
  mPoint2.writeToXml(document, pt2Node);

  rapidxml::xml_node<> * pt3Node = xmlutils::createNode(document, "Point3");
  mPoint3.writeToXml(document, pt3Node);
}

void TriangleShape::setPosition(const Point3D & pt1, const Point3D & pt2, const Point3D & pt3)
{
  setPoint1(pt1);
  setPoint2(pt2);
  setPoint3(pt3);
}

namespace  {
  const bool r = Shape::factory().registerType(OBJECT_ID, boost::bind(boost::factory<TriangleShape*>()));
}

}
}
