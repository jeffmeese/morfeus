#include "perfectelectricconductor.h"

namespace morfeus {
namespace model {
namespace media {

PerfectElectricConductor::PerfectElectricConductor()
  : Dielectric ("Perfect Electric Conductor")
{
  setPermittivity(math::dcomplex(0.0,0.0));
  setPermeability(math::dcomplex(1.0, 0.0));
}

void PerfectElectricConductor::doPrint(std::ostream & output, int tabPos) const
{

}

math::dcomplex PerfectElectricConductor::doValue(double freqGHz) const
{
  math::dcomplex value(0.0,0.0);
  return value;
}

void PerfectElectricConductor::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{

}

void PerfectElectricConductor::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{

}

}
}
}
