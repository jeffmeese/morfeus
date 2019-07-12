#include "shape.h"

Shape::Shape()
{
}

std::string Shape::name() const
{
  return mName;
}

void Shape::setName(const std::string &name)
{
  mName = name;
}
