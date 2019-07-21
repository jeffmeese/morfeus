#include "shape.h"

Shape::Shape()
{
}

Shape::Shape(int32_t number)
  : MorfeusObject (number)
{

}

Shape::Shape(const std::string & name, int32_t number)
  : MorfeusObject (name, number)
{

}

std::vector<Segment> Shape::getSegmentList() const
{
  return doGetSegmentList();
}

std::vector<Vertex> Shape::getVertexList() const
{
  return doGetVertexList();
}

