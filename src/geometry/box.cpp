#include "box.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

namespace morfeus {
namespace geometry {

static const std::string OBJECT_ID("Box");

Box::Box()
  : Shape(OBJECT_ID)
{
  setCenter(Point3D(0.0,0.0,0.0));
  setHeight(1.0);
  setWidth(1.0);
  setLength(1.0);
}

Box::Box(const Point3D & center, double length, double width, double height)
  : Shape(OBJECT_ID)
{
  setCenter(center);
  setHeight(height);
  setWidth(width);
  setLength(length);
}

Box::Box(const std::string & name)
  : Shape(OBJECT_ID, name)
{
  setCenter(Point3D(0.0,0.0,0.0));
  setHeight(1.0);
  setWidth(1.0);
  setLength(1.0);
}

Box::Box(const std::string & name, const Point3D & center, double length, double width, double height)
  : Shape(OBJECT_ID, name)
{
  setCenter(center);
  setHeight(height);
  setWidth(width);
  setLength(length);
}

std::vector<Face> Box::doGetFacetList() const
{
  std::vector<Face> facets;

  Face f1;
  f1.addSegment(Segment(0, 1));
  f1.addSegment(Segment(1, 2));
  f1.addSegment(Segment(2, 3));
  f1.addSegment(Segment(3, 0));
  facets.push_back(f1);

  Face f2;
  f2.addSegment(Segment(4, 5));
  f2.addSegment(Segment(5, 6));
  f2.addSegment(Segment(6, 7));
  f2.addSegment(Segment(7, 4));
  facets.push_back(f2);

  Face f3;
  f3.addSegment(Segment(5, 6));
  f3.addSegment(Segment(6, 2));
  f3.addSegment(Segment(2, 1));
  f3.addSegment(Segment(1, 5));
  facets.push_back(f3);

  Face f4;
  f4.addSegment(Segment(6, 7));
  f4.addSegment(Segment(7, 3));
  f4.addSegment(Segment(3, 2));
  f4.addSegment(Segment(2, 6));
  facets.push_back(f4);

  Face f5;
  f5.addSegment(Segment(7, 4));
  f5.addSegment(Segment(4, 0));
  f5.addSegment(Segment(0, 3));
  f5.addSegment(Segment(3, 7));
  facets.push_back(f5);

  Face f6;
  f6.addSegment(Segment(4, 5));
  f6.addSegment(Segment(5, 1));
  f6.addSegment(Segment(1, 0));
  f6.addSegment(Segment(0, 4));
  facets.push_back(f6);


  return facets;
}

std::vector<Segment> Box::doGetSegmentList() const
{
  std::vector<Segment> segments;
  return segments;
}

std::vector<Vertex> Box::doGetVertexList() const
{
  std::vector<Vertex> vertexList;

  // Calculate the bounds of the box
  double xl = mCenter.x() - mWidth/2;
  double xu = mCenter.x() + mWidth/2;
  double yl = mCenter.y() - mLength/2;
  double yu = mCenter.y() + mLength/2;
  double zl = mCenter.z() - mHeight/2;
  double zu = mCenter.z() + mHeight/2;

  // Create the vertices of the box
  Vertex v1(0, xl, yl, zl);
  Vertex v2(1, xu, yl, zl);
  Vertex v3(2, xu, yu, zl);
  Vertex v4(3, xl, yu, zl);
  Vertex v5(4, xl, yl, zu);
  Vertex v6(5, xu, yl, zu);
  Vertex v7(6, xu, yu, zu);
  Vertex v8(7, xl, yu, zu);

  vertexList.push_back(v1);
  vertexList.push_back(v2);
  vertexList.push_back(v3);
  vertexList.push_back(v4);
  vertexList.push_back(v5);
  vertexList.push_back(v6);
  vertexList.push_back(v7);
  vertexList.push_back(v8);

  return vertexList;
}

void Box::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printValue(output, tabPos, "x: ", mCenter.x());
  xmlutils::printValue(output, tabPos, "y: ", mCenter.y());
  xmlutils::printValue(output, tabPos, "z: ", mCenter.z());
  xmlutils::printValue(output, tabPos, "length: ", mLength);
  xmlutils::printValue(output, tabPos, "width: ", mWidth);
  xmlutils::printValue(output, tabPos, "height: ", mHeight);
}

void Box::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  double x = xmlutils::readAttribute<double>(node, "x");
  double y = xmlutils::readAttribute<double>(node, "y");
  double z = xmlutils::readAttribute<double>(node, "z");

  setCenter(Point3D(x, y, z));
  setLength(xmlutils::readAttribute<double>(node, "length"));
  setWidth(xmlutils::readAttribute<double>(node, "width"));
  setHeight(xmlutils::readAttribute<double>(node, "height"));
}

void Box::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "length", mLength);
  xmlutils::writeAttribute(document, node, "width", mWidth);
  xmlutils::writeAttribute(document, node, "height", mHeight);
  rapidxml::xml_node<> * centerNode = xmlutils::createNode(document, "Center");
  mCenter.writeToXml(document, centerNode);
}

namespace  {
  const bool r = Shape::factory().registerType(OBJECT_ID, boost::bind(boost::factory<Box*>()));
}

}
}
