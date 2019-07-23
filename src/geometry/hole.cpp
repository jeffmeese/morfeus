#include "hole.h"
#include "geometry.h"

static const std::string OBJECT_ID("Hole");

Hole::Hole()
  : MorfeusObject (OBJECT_ID)
{
  setPosition(0.0, 0.0);
}

Hole::Hole(const std::string & name)
  : MorfeusObject (OBJECT_ID)
  , mName(name)
{
  setPosition(0.0, 0.0);
}

Hole::Hole(const std::string & id, const std::string & name)
  : MorfeusObject (OBJECT_ID, id)
  , mName(name)
{
  setPosition(0.0, 0.0);
}

Hole::Hole(double x, double y)
  : MorfeusObject (OBJECT_ID)
{
  setPosition(x, y);
}

Hole::Hole(const std::string & name, double x, double y)
  : MorfeusObject (OBJECT_ID)
  , mName(name)
{
  setPosition(x, y);
}

Hole::Hole(const std::string & id, const std::string & name, double x, double y)
  : MorfeusObject (OBJECT_ID, id)
  , mName(name)
{
  setPosition(x, y);
}

void Hole::print(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
  xmlutils::printValue(output, tabPos, "Name: ", mName);
  xmlutils::printValue(output, tabPos, "x: ", mX);
  xmlutils::printValue(output, tabPos, "y: ", mY);
}

void Hole::print(int tabPos) const
{
  print(std::cout, tabPos);
}

void Hole::readFromXml(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  setX(xmlutils::readAttribute<double>(node, "x"));
  setY(xmlutils::readAttribute<double>(node, "y"));
}

void Hole::writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  rapidxml::xml_node<> * childNode = xmlutils::createNode(document, "Hole");
  xmlutils::writeAttribute(document, childNode, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, childNode, "x", mX);
  xmlutils::writeAttribute(document, childNode, "y", mY);
  node->append_node(childNode);
}

void Hole::setPosition(double x, double y)
{
  setX(x);
  setY(y);
}
