#include "node.h"

namespace morfeus {
namespace mesh {

static const std::string OBJECT_ID("Node");

Node::Node(int32_t number)
  : MorfeusObject(OBJECT_ID)
  , mNumber(number)
{
  setPosition(0.0, 0.0, 0.0);
  setBoundary(false);
  setScaling(1.0, 1.0, 1.0);
  setTranslation(0.0, 0.0, 0.0);
}

Node::Node(int32_t number, double x, double y, double z)
  : MorfeusObject(OBJECT_ID)
  , mNumber(number)
{
  setPosition(x, y, z);
  setBoundary(false);
  setScaling(1.0, 1.0, 1.0);
  setTranslation(0.0, 0.0, 0.0);
}

void Node::doPrint(std::ostream &output, int tabPos) const
{
}

void Node::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
}

void Node::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
}

}
}
