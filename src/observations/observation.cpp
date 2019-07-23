#include "observation.h"

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

void Observation::calculate(double freqGHz, const Mesh *mesh, const MeshInformation *meshInfo, const vector &efield)
{
  doCalculate(freqGHz, mesh, meshInfo, efield);
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

void Observation::readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  setName(xmlutils::readAttribute<std::string>(node, "name"));
  doXmlRead(document, node);
}

void Observation::writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  rapidxml::xml_node<> * childNode = xmlutils::createNode(document, "Observation");
  xmlutils::writeAttribute(document, childNode, "name", mName);
  doXmlWrite(document, childNode);
  node->append_node(childNode);
}
