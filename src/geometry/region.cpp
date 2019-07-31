#include "region.h"

static const std::string OBJECT_ID("Region");

namespace morfeus {
namespace geometry {

Region::Region()
  : MorfeusObject (OBJECT_ID)
{
  setPosition(Point(0.0, 0.0, 0.0));
  setAttribute(0.0);
  setMaxArea(-1.0);
  setMedium(nullptr);
}

Region::Region(const std::string & name)
  : MorfeusObject (OBJECT_ID)
  , mName(name)
{
  setPosition(Point(0.0, 0.0, 0.0));
  setAttribute(0.0);
  setMaxArea(-1.0);
  setMedium(nullptr);
}

Region::Region(const std::string & id, const std::string & name)
  : MorfeusObject (OBJECT_ID, id)
  , mName(name)
{
  setPosition(Point(0.0, 0.0, 0.0));
  setAttribute(0.0);
  setMaxArea(-1.0);
  setMedium(nullptr);
}

Region::Region(const Point & pt)
  : MorfeusObject (OBJECT_ID)
{
  setPosition(pt);
  setAttribute(0.0);
  setMaxArea(-1.0);
  setMedium(nullptr);
}

Region::Region(const std::string & name, const Point & pt)
  : MorfeusObject (OBJECT_ID)
  , mName(name)
{
  setPosition(pt);
  setAttribute(0.0);
  setMaxArea(-1.0);
  setMedium(nullptr);
}

Region::Region(const std::string & id, const std::string & name, const Point & pt)
  : MorfeusObject (OBJECT_ID, id)
  , mName(name)
{
  setPosition(pt);
  setAttribute(0.0);
  setMaxArea(-1.0);
  setMedium(nullptr);
}

double Region::attribute() const
{
  return mAttribute;
}

void Region::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printValue(output, tabPos, "Name: ", mName);
  xmlutils::printValue(output, tabPos+2, "x: ", mPosition.x());
  xmlutils::printValue(output, tabPos+2, "y: ", mPosition.y());
  xmlutils::printValue(output, tabPos+2, "z: ", mPosition.z());
  xmlutils::printValue(output, tabPos+2, "Attribute: ", mAttribute);
  xmlutils::printValue(output, tabPos+2, "Max Area: ", mMaxArea);
}

void Region::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  setName(xmlutils::readAttribute<std::string>(node, "name"));
  setAttribute(xmlutils::readAttribute<double>(node, "attribute"));
  setMaxArea(xmlutils::readAttribute<double>(node, "max-area"));
  rapidxml::xml_node<> * posNode = node->first_node("Position", 0, false);
  if (posNode != nullptr) {
    mPosition.readFromXml(document, posNode);
  }
}

void Region::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, node, "name", mName);
  xmlutils::writeAttribute(document, node, "max-area", mMaxArea);
  xmlutils::writeAttribute(document, node, "attribute", mAttribute);
  rapidxml::xml_node<> * posNode = xmlutils::createNode(document, "Position");
  mPosition.writeToXml(document, posNode);
}

}
}
