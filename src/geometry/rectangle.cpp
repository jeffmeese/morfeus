#include "rectangle.h"

#include "geometry.h"
#include "segment.h"
#include "vertex.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

static const std::string OBJECT_ID("Rectangle");

Rectangle::Rectangle()
  : Shape (OBJECT_ID)
{
  setPosition(0.0,0.0,0.0,0.0);
}

Rectangle::Rectangle(const std::string & name)
  : Shape (OBJECT_ID, name)
{
  setPosition(0.0,0.0,0.0,0.0);
}

Rectangle::Rectangle(const std::string & id, const std::string & name)
  : Shape (OBJECT_ID, id, name)
{
  setPosition(0.0,0.0,0.0,0.0);
}

Rectangle::Rectangle(double xl, double xu, double yl, double yu)
  : Shape (OBJECT_ID)
{
  setPosition(xl, xu, yl, yu);
}

Rectangle::Rectangle(const std::string & name, double xl, double xu, double yl, double yu)
  : Shape (OBJECT_ID, name)
{
  setPosition(xl, xu, yl, yu);
}

Rectangle::Rectangle(const std::string & id, const std::string & name, double xl, double xu, double yl, double yu)
  : Shape (OBJECT_ID, id, name)
{
  setPosition(xl, xu, yl, yu);
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
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
  xmlutils::printValue(output, tabPos, "xl: ", mXl);
  xmlutils::printValue(output, tabPos, "xu: ", mXu);
  xmlutils::printValue(output, tabPos, "yl: ", mYl);
  xmlutils::printValue(output, tabPos, "yu: ", mYu);
}

void Rectangle::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  setXl(xmlutils::readAttribute<double>(node, "xl"));
  setXu(xmlutils::readAttribute<double>(node, "xu"));
  setYl(xmlutils::readAttribute<double>(node, "yl"));
  setYu(xmlutils::readAttribute<double>(node, "yu"));
}

void Rectangle::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
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
  const bool r = Shape::factory().registerType(OBJECT_ID, boost::bind(boost::factory<Rectangle*>()));
}
