#include "segment.h"

Segment::Segment()
  : mNode1(-1)
  , mNode2(-1)
{
  setBoundary(false);
}

Segment::Segment(int32_t node1, int32_t node2)
  : mNode1(node1)
  , mNode2(node2)
{
  setBoundary(false);
}

Segment::Segment(int32_t number, int32_t node1, int32_t node2)
  : mNode1(node1)
  , mNode2(node2)
{
  setNumber(number);
  setBoundary(false);
}

bool Segment::boundary() const
{
  return mBoundary;
}

void Segment::doPrint(std::ostream &output, int tabPos) const
{

}

void Segment::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{

}

void Segment::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
}

int32_t Segment::node1() const
{
  return mNode1;
}

int32_t Segment::node2() const
{
  return mNode2;
}

void Segment::setBoundary(bool value)
{
  mBoundary = value;
}

void Segment::setNode1(int32_t node1)
{
  mNode1 = node1;
}

void Segment::setNode2(int32_t node2)
{
  mNode2 = node2;
}

void Segment::setNodes(int32_t node1, int32_t node2)
{
  setNode1(node1);
  setNode2(node2);
}
