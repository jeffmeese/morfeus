#include "triangleshape.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

static const std::string OBJECT_ID("Triangle");

TriangleShape::TriangleShape()
  : Shape(OBJECT_ID)
{
  init(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}

TriangleShape::TriangleShape(const std::string & name)
  : Shape (OBJECT_ID, name)
{
  init(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}

TriangleShape::TriangleShape(const std::string & id, const std::string & name)
  : Shape(OBJECT_ID, id, name)
{
  init(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}

TriangleShape::TriangleShape(double x1, double x2, double x3, double y1, double y2, double y3)
  : Shape(OBJECT_ID)
{
  init(x1, x2, x3, y1, y2, y3);
}

TriangleShape::TriangleShape(const std::string & name, double x1, double x2, double x3, double y1, double y2, double y3)
  : Shape (OBJECT_ID, name)
{
  init(x1, x2, x3, y1, y2, y3);
}

TriangleShape::TriangleShape(const std::string & id, const std::string & name, double x1, double x2, double x3, double y1, double y2, double y3)
  : Shape (OBJECT_ID, id, name)
{
  init(x1, x2, x3, y1, y2, y3);
}

std::vector<Segment> TriangleShape::doGetSegmentList() const
{
  std::vector<Segment> segmentList;

  segmentList.push_back(Segment(0, 0, 1));
  segmentList.push_back(Segment(0, 1, 2));
  segmentList.push_back(Segment(0, 2, 0));
  return segmentList;
}

std::vector<Vertex> TriangleShape::doGetVertexList() const
{
  std::vector<Vertex> vertexList;

  vertexList.push_back(Vertex(0, mX1, mY1));
  vertexList.push_back(Vertex(1, mX2, mY2));
  vertexList.push_back(Vertex(2, mX3, mY3));
  return vertexList;
}

void TriangleShape::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
  xmlutils::printValue(output, tabPos, "x1", mX1);
  xmlutils::printValue(output, tabPos, "x2", mX2);
  xmlutils::printValue(output, tabPos, "x3", mX3);
  xmlutils::printValue(output, tabPos, "y1", mY1);
  xmlutils::printValue(output, tabPos, "y2", mY2);
  xmlutils::printValue(output, tabPos, "y3", mY3);
}

void TriangleShape::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  setX1(xmlutils::readAttribute<double>(node, "x1"));
  setX2(xmlutils::readAttribute<double>(node, "x2"));
  setX3(xmlutils::readAttribute<double>(node, "x3"));
  setY1(xmlutils::readAttribute<double>(node, "y1"));
  setY2(xmlutils::readAttribute<double>(node, "y2"));
  setY3(xmlutils::readAttribute<double>(node, "y3"));
}

void TriangleShape::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, node, "x1", mX1);
  xmlutils::writeAttribute(document, node, "x2", mX2);
  xmlutils::writeAttribute(document, node, "x3", mX3);
  xmlutils::writeAttribute(document, node, "x1", mY1);
  xmlutils::writeAttribute(document, node, "x2", mY2);
  xmlutils::writeAttribute(document, node, "x3", mY3);
}

void TriangleShape::init(double x1, double x2, double x3, double y1, double y2, double y3)
{
  mX1 = x1;
  mX2 = x2;
  mX3 = x3;
  mY1 = y1;
  mY2 = y2;
  mY3 = y3;
}

namespace  {
  const bool r = Shape::factory().registerType(OBJECT_ID, boost::bind(boost::factory<TriangleShape*>()));
}
