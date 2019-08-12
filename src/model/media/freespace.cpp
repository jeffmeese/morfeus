#include "freespace.h"

namespace morfeus {
namespace model {
namespace media {

FreeSpace::FreeSpace()
: Dielectric ("FreeSpace", "Free Space")
{
  setPermittivity(math::dcomplex(1.0,0.0));
  setPermeability(math::dcomplex(1.0, 0.0));
}

void FreeSpace::doPrint(std::ostream & output, int tabPos) const
{

}

math::dcomplex FreeSpace::doValue(double freqGHz) const
{
  math::dcomplex value(0.0,0.0);
  return value;
}

void FreeSpace::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{

}

void FreeSpace::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{

}

}
}
}
