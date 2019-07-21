#include "triangleshape.h"

static const std::string OBJECT_ID("Triangle");

TriangleShape::TriangleShape()
{
  init(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}

TriangleShape::TriangleShape(int32_t number)
  : Polygon(number)
{
  init(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}

TriangleShape::TriangleShape(const std::string & name, int32_t number)
  : Polygon (name, number)
{
  init(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}

TriangleShape::TriangleShape(double x1, double x2, double x3, double y1, double y2, double y3)
{
  init(x1, x2, x3, y1, y2, y3);
}

TriangleShape::TriangleShape(int32_t number, double x1, double x2, double x3, double y1, double y2, double y3)
  : Polygon (number)
{
  init(x1, x2, x3, y1, y2, y3);
}

TriangleShape::TriangleShape(const std::string & name, int32_t number, double x1, double x2, double x3, double y1, double y2, double y3)
  : Polygon (name, number)
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
  xmlutils::printHeader(output, tabPos, "Triangle");
  xmlutils::printValue(output, tabPos+2, "Name: ", name());
  xmlutils::printValue(output, tabPos+2, "Number: ", number());
  xmlutils::printValue(output, tabPos+2, "x1", mX1);
  xmlutils::printValue(output, tabPos+2, "x2", mX2);
  xmlutils::printValue(output, tabPos+2, "x3", mX3);
  xmlutils::printValue(output, tabPos+2, "y1", mY1);
  xmlutils::printValue(output, tabPos+2, "y2", mY2);
  xmlutils::printValue(output, tabPos+2, "y3", mY3);
}

void TriangleShape::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  setName(xmlutils::readAttribute<std::string>(node, "name"));
  setNumber(std::stoi(xmlutils::readAttribute<std::string>(node, "number")));
  setX1(std::stod(xmlutils::readAttribute<std::string>(node, "x1")));
  setX2(std::stod(xmlutils::readAttribute<std::string>(node, "x2")));
  setX3(std::stod(xmlutils::readAttribute<std::string>(node, "x3")));
  setY1(std::stod(xmlutils::readAttribute<std::string>(node, "y1")));
  setY2(std::stod(xmlutils::readAttribute<std::string>(node, "y2")));
  setY3(std::stod(xmlutils::readAttribute<std::string>(node, "y3")));
}

void TriangleShape::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "name", name());
  xmlutils::writeAttribute(document, node, "number", number());
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
  Shape * createFunc()
  {
    return new TriangleShape;
  }

  const bool registered = Shape::Factory::Instance().Register(OBJECT_ID, createFunc);
}

