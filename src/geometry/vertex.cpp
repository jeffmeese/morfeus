#include "vertex.h"

Vertex::Vertex()
{
  setPosition(0.0, 0.0);
  setBoundary(false);
}

Vertex::Vertex(double x, double y)
{
  setPosition(x, y);
  setBoundary(false);
}

Vertex::Vertex(int32_t number, double x, double y)
{
  setNumber(number);
  setPosition(x, y);
  setBoundary(false);
}

void Vertex::addAttribute(double value)
{
  mAttributes.push_back(value);
}

void Vertex::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, "Vertex");
  xmlutils::printValue(output, tabPos+2, "Name: ", name());
  xmlutils::printValue(output, tabPos+2, "Number: ", number());
  xmlutils::printValue(output, tabPos+2, "x", mX);
  xmlutils::printValue(output, tabPos+2, "y", mY);
}

void Vertex::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  setName(xmlutils::readAttribute<std::string>(node, "name"));
  setNumber(std::stoi(xmlutils::readAttribute<std::string>(node, "number")));
  setX(std::stod(xmlutils::readAttribute<std::string>(node, "x")));
  setY(std::stod(xmlutils::readAttribute<std::string>(node, "y")));
}

void Vertex::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "name", name());
  xmlutils::writeAttribute(document, node, "number", number());
  xmlutils::writeAttribute(document, node, "type", "Vertex");
  xmlutils::writeAttribute(document, node, "x", mX);
  xmlutils::writeAttribute(document, node, "y", mY);
}

