#include "polygon.h"

static const std::string OBJECT_ID("Polygon");

Polygon::Polygon()
{

}

Polygon::Polygon(int32_t number)
  : Shape(number)
{

}

Polygon::Polygon(const std::string & name, int32_t number)
  : Shape(name, number)
{

}

void Polygon::addPoint(double x, double y)
{
  addPoint(Point2D(x, y));
}

void Polygon::addPoint(const Point2D & point)
{
  mPoints.push_back(point);
}

std::vector<Segment> Polygon::doGetSegmentList() const
{
  std::vector<Segment> segmentList;
  return segmentList;
}

std::vector<Vertex> Polygon::doGetVertexList() const
{
  std::vector<Vertex> vertexList;
  for (std::size_t i = 0; i < mPoints.size(); i++) {
    const Point2D & pt = mPoints.at(i);
    vertexList.push_back(Vertex(static_cast<int32_t>(i), pt.x(), pt.y()));
  }
  return vertexList;
}

void Polygon::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, "Polygon");
  xmlutils::printValue(output, tabPos+2, "Name: ", name());
  xmlutils::printValue(output, tabPos+2, "Number: ", number());
  xmlutils::printHeader(output, tabPos+2, "Points");
  for (std::size_t i = 0; i < mPoints.size(); i++) {
    xmlutils::printValue(output, tabPos+4, "Point: ", mPoints.at(i));
  }
}

void Polygon::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  setName(xmlutils::readAttribute<std::string>(node, "name"));
  setNumber(std::stoi(xmlutils::readAttribute<std::string>(node, "number")));
}

void Polygon::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "name", name());
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, node, "number", number());
  for (std::size_t i = 0; i < mPoints.size(); i++) {
    rapidxml::xml_node<> * pointNode = xmlutils::createNode(document, "point");
    xmlutils::writeAttribute(document, pointNode, "x", mPoints.at(i).x());
    xmlutils::writeAttribute(document, pointNode, "y", mPoints.at(i).y());
    node->append_node(pointNode);
  }
}

namespace  {
  Shape * createFunc()
  {
    return new Polygon;
  }

  const bool registered = Shape::Factory::Instance().Register(OBJECT_ID, createFunc);
}
