#include "rectangle.h"

#include "geometry/model.h"
#include "geometry/segment.h"
#include "geometry/vertex.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

namespace Morfeus {
namespace Geometry {

static const std::string OBJECT_ID("Rectangle");

Rectangle::Rectangle()
  : Shape (OBJECT_ID)
{
  setPosition(0.0,0.0,0.0,0.0);
  setZ(0.0);
}

Rectangle::Rectangle(const std::string & name)
  : Shape (OBJECT_ID, name)
{
  setPosition(0.0,0.0,0.0,0.0);
  setZ(0.0);
}

Rectangle::Rectangle(const std::string & id, const std::string & name)
  : Shape (OBJECT_ID, id, name)
{
  setPosition(0.0,0.0,0.0,0.0);
  setZ(0.0);
}

Rectangle::Rectangle(double xl, double xu, double yl, double yu)
  : Shape (OBJECT_ID)
{
  setPosition(xl, xu, yl, yu);
  setZ(0.0);
}

Rectangle::Rectangle(const std::string & name, double xl, double xu, double yl, double yu)
  : Shape (OBJECT_ID, name)
{
  setPosition(xl, xu, yl, yu);
  setZ(0.0);
}

Rectangle::Rectangle(const std::string & id, const std::string & name, double xl, double xu, double yl, double yu)
  : Shape (OBJECT_ID, id, name)
{
  setPosition(xl, xu, yl, yu);
  setZ(0.0);
}

std::vector<Face> Rectangle::doGetFacetList() const
{
  std::vector<Face> facets;
  return facets;
}

std::vector<Segment> Rectangle::doGetSegmentList() const
{
  std::vector<Segment> segments;
  return segments;
}

std::vector<Vertex> Rectangle::doGetVertexList() const
{
  std::vector<Vertex> vertexList;
  return vertexList;
}

void Rectangle::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
  xmlutils::printValue(output, tabPos, "xl: ", mXl);
  xmlutils::printValue(output, tabPos, "xu: ", mXu);
  xmlutils::printValue(output, tabPos, "yl: ", mYl);
  xmlutils::printValue(output, tabPos, "yu: ", mYu);
  xmlutils::printValue(output, tabPos, "z: ", mZ);
}

void Rectangle::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  setXl(xmlutils::readAttribute<double>(node, "xl"));
  setXu(xmlutils::readAttribute<double>(node, "xu"));
  setYl(xmlutils::readAttribute<double>(node, "yl"));
  setYu(xmlutils::readAttribute<double>(node, "yu"));
  setZ(xmlutils::readAttribute<double>(node, "z"));
}

void Rectangle::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, node, "xl", mXl);
  xmlutils::writeAttribute(document, node, "xu", mXu);
  xmlutils::writeAttribute(document, node, "yl", mYl);
  xmlutils::writeAttribute(document, node, "yu", mYu);
  xmlutils::writeAttribute(document, node, "z", mZ);
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

}
}
