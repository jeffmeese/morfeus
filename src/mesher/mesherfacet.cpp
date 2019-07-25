#include "mesherfacet.h"

MesherFacet::MesherFacet()
{
}

void MesherFacet::addPolygon(const MesherPolygon &polygon)
{
  mPolygons.push_back(polygon);
}
