#include "perfectelectricconductor.h"

namespace morfeus {
namespace media {

PerfectElectricConductor::PerfectElectricConductor()
  : Medium ("PerfectElectricConductor", "Perfect Electric Conductor")
{
}

void PerfectElectricConductor::doPrint(std::ostream & output, int tabPos) const
{

}

dcomplex PerfectElectricConductor::doValue(double freqGHz) const
{
  dcomplex value(0.0,0.0);
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
