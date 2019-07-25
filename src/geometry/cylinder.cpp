#include "cylinder.h"

static const std::string OBJECT_ID("Box");

Cylinder::Cylinder()
  : Shape(OBJECT_ID)
{
}

Cylinder::Cylinder(const std::string & name)
  : Shape(OBJECT_ID, name)
{
}

std::vector<MesherPolygon> Cylinder::doGetMesherPolygons() const
{
  std::vector<MesherPolygon> polygons;
  return polygons;
}

void Cylinder::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
  xmlutils::printValue(output, tabPos, "x: ", mX);
  xmlutils::printValue(output, tabPos, "y: ", mY);
  xmlutils::printValue(output, tabPos, "z: ", mZ);
  xmlutils::printValue(output, tabPos, "radius: ", mRadius);
  xmlutils::printValue(output, tabPos, "height: ", mHeight);
  xmlutils::printValue(output, tabPos, "resolution: ", mResolution);
}

void Cylinder::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  setX(xmlutils::readAttribute<double>(node, "x"));
  setY(xmlutils::readAttribute<double>(node, "y"));
  setZ(xmlutils::readAttribute<double>(node, "z"));
  setRadius(xmlutils::readAttribute<double>(node, "radius"));
  setHeight(xmlutils::readAttribute<double>(node, "height"));
  setResolution(xmlutils::readAttribute<std::size_t>(node, "resolution"));
}

void Cylinder::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, node, "x", mX);
  xmlutils::writeAttribute(document, node, "y", mY);
  xmlutils::writeAttribute(document, node, "z", mZ);
  xmlutils::writeAttribute(document, node, "radius", mRadius);
  xmlutils::writeAttribute(document, node, "height", mHeight);
  xmlutils::writeAttribute(document, node, "resolution", mResolution);
}

