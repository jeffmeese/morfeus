#include "vertex.h"

static const std::string OBJECT_ID("Vertex");

Vertex::Vertex()
  : MorfeusObject (OBJECT_ID)
{
  setPosition(0.0, 0.0);
  setBoundary(false);
}

Vertex::Vertex(double x, double y)
  : MorfeusObject (OBJECT_ID)
{
  setPosition(x, y);
  setBoundary(false);
}

Vertex::Vertex(int32_t number, double x, double y)
  : MorfeusObject (OBJECT_ID)
{
  setNumber(number);
  setPosition(x, y);
  setBoundary(false);
}

void Vertex::addAttribute(double value)
{
  mAttributes.push_back(value);
}

void Vertex::print(std::ostream &output, int tabPos) const
{
  xmlutils::printValue(output, tabPos, "Number: ", mNumber);
  xmlutils::printValue(output, tabPos, "x", mX);
  xmlutils::printValue(output, tabPos, "y", mY);
}

void Vertex::print(int tabPos) const
{
  print(std::cout, tabPos);
}

void Vertex::readFromXml(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  setNumber(xmlutils::readAttribute<int32_t>(node, "number"));
  setX(xmlutils::readAttribute<double>(node, "x"));
  setY(xmlutils::readAttribute<double>(node, "y"));
}

void Vertex::writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "number", mNumber);
  xmlutils::writeAttribute(document, node, "x", mX);
  xmlutils::writeAttribute(document, node, "y", mY);
}

void Vertex::setPosition(double x, double y)
{
  setX(x);
  setY(y);
}
