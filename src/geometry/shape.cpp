#include "shape.h"

Shape::Shape()
{
}

void Shape::addToGeometry(Geometry * geometry) const
{
  doAddToGeometry(geometry);
}

