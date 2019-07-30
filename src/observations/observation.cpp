#include "observation.h"

namespace morfeus {
namespace observation {

Observation::Observation(const std::string & type)
  : MorfeusObject (type)
{

}

Observation::Observation(const std::string & type, const std::string & name)
  : MorfeusObject (type)
  , mName(name)
{
}

Observation::Observation(const std::string & type, const std::string & id, const std::string & name)
  : MorfeusObject (type, id)
  , mName(name)
{
}

void Observation::calculate(double freqGHz, double thetaInc, double phiInc, const mesh::Mesh *mesh, const MeshInformation *meshInfo, const vector &efield)
{
  doCalculate(freqGHz, thetaInc, phiInc, mesh, meshInfo, efield);
}

Observation::ObservationFactory & Observation::factory()
{
  static ObservationFactory f;
  return f;
}

Observation * Observation::ObservationFactory::create(const std::string & type)
{
  Observation * object = mFactory.create(type);
  return object;
}

bool Observation::ObservationFactory::registerType(const std::string & type, boost::function<Observation*()> creator)
{
  mFactory.registerFactory(type, creator);
  return true;
}

void Observation::print(std::ostream & output, int tabPos) const
{
  xmlutils::printValue(output, tabPos, "Name: ", mName);
  doPrint(output, tabPos);
}

void Observation::print(int tabPos) const
{
  print(std::cout);
}

void Observation::report(std::ostream &output) const
{
  doReport(output);
}

void Observation::readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  setName(xmlutils::readAttribute<std::string>(node, "name"));
  doXmlRead(document, node);
}

void Observation::writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "name", mName);
  doXmlWrite(document, node);
}

}
}
