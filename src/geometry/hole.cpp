#include "hole.h"
#include "geometry.h"

Hole::Hole()
{
  setPosition(0.0, 0.0);
}

Hole::Hole( double x, double y)
{
  setPosition(x, y);
}

void Hole::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, "Hole");
  xmlutils::printValue(output, tabPos+2, "Name: ", name());
  xmlutils::printValue(output, tabPos+2, "Number: ", number());
  xmlutils::printValue(output, tabPos+2, "x: ", mX);
  xmlutils::printValue(output, tabPos+2, "y: ", mY);
}

void Hole::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  setName(xmlutils::readAttribute<std::string>(node, "name"));
  setNumber(std::stoi(xmlutils::readAttribute<std::string>(node, "number")));
  setX(std::stod(xmlutils::readAttribute<std::string>(node, "x")));
  setY(std::stod(xmlutils::readAttribute<std::string>(node, "y")));
}

void Hole::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "name", name());
  xmlutils::writeAttribute(document, node, "type", "Hole");
  xmlutils::writeAttribute(document, node, "number", number());
  xmlutils::writeAttribute(document, node, "x", mX);
  xmlutils::writeAttribute(document, node, "y", mY);
}

void Hole::setPosition(double x, double y)
{
  setX(x);
  setY(y);
}

void Hole::setX(double value)
{
  mX = value;
}

void Hole::setY(double value)
{
  mY = value;
}

double Hole::x() const
{
  return mX;
}

double Hole::y() const
{
  return mY;
}
