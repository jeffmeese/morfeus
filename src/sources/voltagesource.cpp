#include "voltagesource.h"

#include "model/ports/port.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

namespace morfeus {
namespace sources {

static const std::string OBJECT_ID("VoltageSource");

VoltageSource::VoltageSource()
  : Source (OBJECT_ID)
  , mPort(nullptr)
{
}

VoltageSource::VoltageSource(const std::string & name)
  : Source (OBJECT_ID, name)
  , mPort(nullptr)
{

}

VoltageSource::VoltageSource(const std::string & id, const std::string & name)
  : Source (OBJECT_ID, id, name)
  , mPort(nullptr)
{

}

void VoltageSource::doExcite(double freqGHz, double thetaInc, double phiInc, const mesh::Mesh * mesh,
              const solution::MeshInformation * meshInfo, math::vector & rhs) const
{

}

void VoltageSource::doPrint(std::ostream & output, int tabPos) const
{
  xmlutils::printValue(output, tabPos, "Voltage: ", mVoltage);
}

void VoltageSource::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  std::string portName = xmlutils::readAttribute<std::string>(node, "port-name");
  setVoltage(xmlutils::readAttribute<double>(node, "voltage"));
}

void VoltageSource::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "voltage", mVoltage);
  xmlutils::writeAttribute(document, node, "port-name", mPort->name());
}

namespace  {
  const bool r = Source::factory().registerType(OBJECT_ID, boost::bind(boost::factory<VoltageSource*>()));
}

}
}
