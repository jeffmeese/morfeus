#include "hole.h"

#include "geometry/model.h"

static const std::string OBJECT_ID("Hole");

namespace morfeus {
namespace geometry {

Hole::Hole()
  : MorfeusObject (OBJECT_ID)
{
  setPosition(Point(0.0, 0.0, 0.0));
}

Hole::Hole(const std::string & name)
  : MorfeusObject (OBJECT_ID)
  , mName(name)
{
  setPosition(Point(0.0, 0.0, 0.0));
}

Hole::Hole(const std::string & id, const std::string & name)
  : MorfeusObject (OBJECT_ID, id)
  , mName(name)
{
  setPosition(Point(0.0, 0.0, 0.0));
}

Hole::Hole(const Point & pt)
  : MorfeusObject (OBJECT_ID)
{
  setPosition(pt);
}

Hole::Hole(const std::string & name, const Point & pt)
  : MorfeusObject (OBJECT_ID)
  , mName(name)
{
  setPosition(pt);
}

Hole::Hole(const std::string & id, const std::string & name, const Point & pt)
  : MorfeusObject (OBJECT_ID, id)
  , mName(name)
{
  setPosition(pt);
}

void Hole::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
  xmlutils::printValue(output, tabPos, "Name: ", mName);
  xmlutils::printValue(output, tabPos, "x: ", mPosition.x());
  xmlutils::printValue(output, tabPos, "y: ", mPosition.y());
  xmlutils::printValue(output, tabPos, "z: ", mPosition.z());
}

void Hole::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  setName(xmlutils::readAttribute<std::string>(node, "name"));
  rapidxml::xml_node<> * posNode = node->first_node("Position", 0, false);
  if (posNode != nullptr) {
    mPosition.readFromXml(document, posNode);
  }
}

void Hole::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, node, "name", mName);
  rapidxml::xml_node<> * posNode = xmlutils::createNode(document, "Position");
  mPosition.writeToXml(document, posNode);
}

}
}
