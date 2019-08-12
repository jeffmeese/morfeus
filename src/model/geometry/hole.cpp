#include "hole.h"

namespace morfeus {
namespace model {
namespace geometry {

int32_t Hole::mTotal = 0;
static const std::string OBJECT_ID("Hole");

Hole::Hole()
  : MorfeusObject (OBJECT_ID)
{
  init(Point(0.0, 0.0, 0.0));
  setName(createName(mNumber));
}

Hole::Hole(const std::string & name)
  : MorfeusObject (OBJECT_ID, name)
{
  init(Point(0.0, 0.0, 0.0));
}

Hole::Hole(const std::string & id, const std::string & name)
  : MorfeusObject (OBJECT_ID, id, name)
{
  init(Point(0.0, 0.0, 0.0));
}

Hole::Hole(const Point & pt)
  : MorfeusObject (OBJECT_ID)
{
  init(pt);
  setName(createName(mNumber));
}

Hole::Hole(const std::string & name, const Point & pt)
  : MorfeusObject (OBJECT_ID, name)
{
  init(pt);
}

Hole::Hole(const std::string & id, const std::string & name, const Point & pt)
  : MorfeusObject (OBJECT_ID, id, name)
{
  init(pt);
}

void Hole::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printValue(output, tabPos, "x: ", mPosition.x());
  xmlutils::printValue(output, tabPos, "y: ", mPosition.y());
  xmlutils::printValue(output, tabPos, "z: ", mPosition.z());
}

void Hole::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  rapidxml::xml_node<> * posNode = node->first_node("Position", 0, false);
  if (posNode != nullptr) {
    mPosition.readFromXml(document, posNode);
  }
}

void Hole::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  rapidxml::xml_node<> * posNode = xmlutils::createNode(document, "Position");
  mPosition.writeToXml(document, posNode);
}

void Hole::init(const Point &pt)
{
  mNumber = mTotal++;
  mPosition = pt;
}

}
}
}
