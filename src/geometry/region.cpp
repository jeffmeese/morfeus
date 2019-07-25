#include "region.h"

static const std::string OBJECT_ID("Region");

Region::Region()
  : MorfeusObject (OBJECT_ID)
{
  setPosition(0.0, 0.0, 0.0);
  setAttribute(0.0);
  setMaxArea(-1.0);
}

Region::Region(const std::string & name)
  : MorfeusObject (OBJECT_ID)
  , mName(name)
{
  setPosition(0.0, 0.0, 0.0);
  setAttribute(0.0);
  setMaxArea(-1.0);
}

Region::Region(const std::string & id, const std::string & name)
  : MorfeusObject (OBJECT_ID, id)
  , mName(name)
{
  setPosition(0.0, 0.0, 0.0);
  setAttribute(0.0);
  setMaxArea(-1.0);
}

Region::Region(double x, double y, double z)
  : MorfeusObject (OBJECT_ID)
{
  setPosition(x, y, z);
  setAttribute(0.0);
  setMaxArea(-1.0);
}

Region::Region(const std::string & name, double x, double y, double z)
  : MorfeusObject (OBJECT_ID)
  , mName(name)
{
  setPosition(x, y, z);
  setAttribute(0.0);
  setMaxArea(-1.0);
}


Region::Region(const std::string & id, const std::string & name, double x, double y, double z)
  : MorfeusObject (OBJECT_ID, id)
  , mName(name)
{
  setPosition(x, y, z);
  setAttribute(0.0);
  setMaxArea(-1.0);
}

double Region::attribute() const
{
  return mAttribute;
}

void Region::print(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
  xmlutils::printValue(output, tabPos, "Name: ", mName);
  xmlutils::printValue(output, tabPos, "x: ", mX);
  xmlutils::printValue(output, tabPos, "y: ", mY);
  xmlutils::printValue(output, tabPos, "z: ", mZ);
}

void Region::print(int tabPos) const
{
  print(std::cout, tabPos);
}

void Region::readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  setName(xmlutils::readAttribute<std::string>(node, "name"));
  setAttribute(xmlutils::readAttribute<double>(node, "attribute"));
  setMaxArea(xmlutils::readAttribute<double>(node, "max-area"));
  setX(xmlutils::readAttribute<double>(node, "x"));
  setY(xmlutils::readAttribute<double>(node, "y"));
  setZ(xmlutils::readAttribute<double>(node, "z"));
}

void Region::setPosition(double x, double y, double z)
{
  setX(x);
  setY(y);
  setZ(z);
}

void Region::writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  rapidxml::xml_node<> * childNode = xmlutils::createNode(document, "Region");
  xmlutils::writeAttribute(document, childNode, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, childNode, "name", mName);
  xmlutils::writeAttribute(document, childNode, "max-area", mMaxArea);
  xmlutils::writeAttribute(document, childNode, "attribute", mAttribute);
  xmlutils::writeAttribute(document, childNode, "x", mX);
  xmlutils::writeAttribute(document, childNode, "y", mY);
  xmlutils::writeAttribute(document, childNode, "z", mZ);
  node->append_node(childNode);
}
