#include "region.h"

Region::Region()
{
  setPosition(0.0, 0.0);
  setAttribute(0.0);
  setMaxArea(-1.0);
}

Region::Region(double x, double y)
{
  setPosition(x, y);
  setAttribute(0.0);
  setMaxArea(-1.0);
}

double Region::attribute() const
{
  return mAttribute;
}

void Region::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, "Region");
  xmlutils::printValue(output, tabPos+2, "Name: ", name());
  xmlutils::printValue(output, tabPos+2, "Number: ", number());
  xmlutils::printValue(output, tabPos+2, "x: ", mX);
  xmlutils::printValue(output, tabPos+2, "y: ", mY);
}

void Region::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{

}

void Region::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "name", name());
  xmlutils::writeAttribute(document, node, "number", number());
  xmlutils::writeAttribute(document, node, "maxArea", mMaxArea);
  xmlutils::writeAttribute(document, node, "attribute", mAttribute);
  xmlutils::writeAttribute(document, node, "x", mX);
  xmlutils::writeAttribute(document, node, "y", mY);
}

double Region::maxArea() const
{
  return mMaxArea;
}

void Region::setAttribute(double value)
{
  mAttribute = value;
}

void Region::setMaxArea(double value)
{
  mMaxArea = value;
}

void Region::setPosition(double x, double y)
{
  setX(x);
  setY(y);
}

void Region::setX(double value)
{
  mX = value;
}

void Region::setY(double value)
{
  mY = value;
}

double Region::x() const
{
  return mX;
}

double Region::y() const
{
  return mY;
}


