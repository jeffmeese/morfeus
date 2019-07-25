#include "circle.h"

#include "xmlutils.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

static const std::string OBJECT_ID("Circle");

Circle::Circle()
  : Shape(OBJECT_ID)
{
  setCenter(0.0, 0.0, 0.0);
  setRadius(1.0);
}

Circle::Circle(const std::string & name)
  : Shape(OBJECT_ID, name)
{
  setCenter(0.0, 0.0, 0.0);
  setRadius(1.0);
}

Circle::Circle(const std::string & id, const std::string & name)
  : Shape(OBJECT_ID, id, name)
{
  setCenter(0.0, 0.0, 0.0);
  setRadius(1.0);
}

Circle::Circle(double x, double y, double z, double radius)
  : Shape(OBJECT_ID)
{
  setCenter(x, y, z);
  setRadius(radius);
}

Circle::Circle(const std::string & name, double x, double y, double z, double radius)
  : Shape(OBJECT_ID, name)
{
  setCenter(x, y, z);
  setRadius(radius);
}

Circle::Circle(const std::string & id, const std::string & name, double x, double y, double z, double radius)
  : Shape(OBJECT_ID, id, name)
{
  setCenter(x, y, z);
  setRadius(radius);
}

std::vector<MesherPolygon> Circle::doGetMesherPolygons() const
{
  std::vector<MesherPolygon> polygons;
  return polygons;
}

void Circle::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
  xmlutils::printValue(output, tabPos, "x: ", mX);
  xmlutils::printValue(output, tabPos, "y: ", mY);
  xmlutils::printValue(output, tabPos, "z: ", mZ);
  xmlutils::printValue(output, tabPos, "Radius: ", mRadius);
  xmlutils::printValue(output, tabPos, "Resolution: ", mResolution);
}

void Circle::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  setRadius(xmlutils::readAttribute<double>(node, "radius"));
  setResolution(xmlutils::readAttribute<std::size_t>(node, "resolution"));
  setX(xmlutils::readAttribute<double>(node, "x"));
  setY(xmlutils::readAttribute<double>(node, "y"));
  setZ(xmlutils::readAttribute<double>(node, "z"));
}

void Circle::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, node, "x", mX);
  xmlutils::writeAttribute(document, node, "y", mY);
  xmlutils::writeAttribute(document, node, "z", mZ);
  xmlutils::writeAttribute(document, node, "radius", mRadius);
  xmlutils::writeAttribute(document, node, "resolution", mResolution);
}

void Circle::setCenter(double x, double y, double z)
{
  mX = x;
  mY = y;
  mZ = z;
}

namespace  {
  const bool r = Shape::factory().registerType(OBJECT_ID, boost::bind(boost::factory<Circle*>()));
}
