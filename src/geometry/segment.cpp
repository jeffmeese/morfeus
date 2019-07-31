#include "segment.h"

namespace morfeus {
namespace geometry {

static const std::string OBJECT_ID("Segment");

Segment::Segment()
  : MorfeusObject (OBJECT_ID)
{
  mNumber = -1;
  mNode1 = mNode2 = 0;
}

Segment::Segment(const std::string & id)
  : MorfeusObject (OBJECT_ID, id)
{
  mNumber = -1;
  mNode1 = mNode2 = 0;
}

Segment::Segment(int32_t number)
  : MorfeusObject (OBJECT_ID)
  , mNumber(number)
{
  mNode1 = mNode2 = 0;
}

Segment::Segment(const std::string & id, int32_t number)
  : MorfeusObject (OBJECT_ID, id)
  , mNumber(number)
{
  mNode1 = mNode2 = 0;
}

Segment::Segment(int32_t node1, int32_t node2)
  : MorfeusObject (OBJECT_ID)
{
  mNumber = -1;
  mNode1 = node1;
  mNode2 = node2;
}

Segment::Segment(const std::string & id, int32_t node1, int32_t node2)
  : MorfeusObject (OBJECT_ID, id)
{
  mNumber = -1;
  mNode1 = node1;
  mNode2 = node2;
}

Segment::Segment(int32_t number, int32_t node1, int32_t node2)
  : MorfeusObject (OBJECT_ID)
  , mNumber(number)
{
  mNode1 = node1;
  mNode2 = node2;
}

Segment::Segment(const std::string & id, int32_t number, int32_t node1, int32_t node2)
  : MorfeusObject (OBJECT_ID, id)
  , mNumber(number)
{
  mNode1 = node1;
  mNode2 = node2;
}

bool Segment::boundary() const
{
  return mBoundary;
}

void Segment::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
  xmlutils::printValue(output, tabPos, "Number: ", mNumber);
  xmlutils::printValue(output, tabPos, "Node 1: ", mNode1);
  xmlutils::printValue(output, tabPos, "Node 2: ", mNode2);
}

void Segment::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  setNumber(xmlutils::readAttribute<int32_t>(node, "number"));
  setNode1(xmlutils::readAttribute<int32_t>(node, "node1"));
  setNode2(xmlutils::readAttribute<int32_t>(node, "node2"));
}

void Segment::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, node, "number", mNumber);
  xmlutils::writeAttribute(document, node, "node1", mNode1);
  xmlutils::writeAttribute(document, node, "node2", mNode1);
}

void Segment::setNodes(int32_t node1, int32_t node2)
{
  setNode1(node1);
  setNode2(node2);
}

}
}
