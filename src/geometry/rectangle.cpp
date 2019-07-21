#include "rectangle.h"

#include "geometry.h"
#include "segment.h"
#include "vertex.h"

static const std::string OBJECT_ID("Rectangle");

Rectangle::Rectangle()
{
}

std::vector<Segment> Rectangle::doGetSegmentList() const
{
  std::vector<Segment> segmentList;

  segmentList.push_back(Segment(0, 1, 2));
  segmentList.push_back(Segment(1, 2, 3));
  segmentList.push_back(Segment(2, 3, 4));
  segmentList.push_back(Segment(3, 4, 1));

  return segmentList;
}

std::vector<Vertex> Rectangle::doGetVertexList() const
{
  std::vector<Vertex> vertexList;
  vertexList.push_back(Vertex(0, mXl, mYl));
  vertexList.push_back(Vertex(1, mXu, mYl));
  vertexList.push_back(Vertex(2, mXu, mYu));
  vertexList.push_back(Vertex(3, mXl, mYu));
  return vertexList;
}

void Rectangle::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, "Rectangle");
  xmlutils::printValue(output, tabPos+2, "Name: ", name());
  xmlutils::printValue(output, tabPos+2, "Number: ", number());
  xmlutils::printValue(output, tabPos+2, "xl: ", mXl);
  xmlutils::printValue(output, tabPos+2, "xu: ", mXu);
  xmlutils::printValue(output, tabPos+2, "yl: ", mYl);
  xmlutils::printValue(output, tabPos+2, "yu: ", mYu);
}

void Rectangle::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  setName(xmlutils::readAttribute<std::string>(node, "name"));
  setNumber(std::stoi(xmlutils::readAttribute<std::string>(node, "number")));
  setXl(std::stod(xmlutils::readAttribute<std::string>(node, "xl")));
  setXu(std::stod(xmlutils::readAttribute<std::string>(node, "xu")));
  setYl(std::stod(xmlutils::readAttribute<std::string>(node, "yl")));
  setYu(std::stod(xmlutils::readAttribute<std::string>(node, "yu")));
}

void Rectangle::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "name", name());
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, node, "number", number());
  xmlutils::writeAttribute(document, node, "xl", mXl);
  xmlutils::writeAttribute(document, node, "xu", mXu);
  xmlutils::writeAttribute(document, node, "yl", mYl);
  xmlutils::writeAttribute(document, node, "yu", mYu);
}

double Rectangle::height() const
{
  return fabs(mYu-mYl);
}

double Rectangle::width() const
{
  return fabs(mXu-mXl);
}

void Rectangle::setPosition(double xl, double xu, double yl, double yu)
{
  mXl = xl;
  mXu = xu;
  mYl = yl;
  mYu = yu;
}

void Rectangle::setPosition(Point2D lowerLeft, Point2D upperRight)
{
  setLowerLeft(lowerLeft);
  setUpperRight(upperRight);
}

namespace  {
  Shape * createFunc()
  {
    return new Rectangle;
  }

  const bool registered = Shape::Factory::Instance().Register(OBJECT_ID, createFunc);
}
