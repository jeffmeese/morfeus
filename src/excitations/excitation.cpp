#include "excitation.h"

namespace Morfeus {

Excitation::Excitation(const std::string & type)
  : MorfeusObject (type)
  , mAngleDependent(false)
{
}

Excitation::Excitation(const std::string & type, const std::string & name)
  : MorfeusObject (type)
  , mName(name)
  , mAngleDependent(false)
{
}

Excitation::Excitation(const std::string & type, const std::string & id, const std::string & name)
  : MorfeusObject (type, id)
  , mName(name)
  , mAngleDependent(false)
{
}

Excitation::ExcitationFactory & Excitation::factory()
{
  static ExcitationFactory f;
  return f;
}

Excitation * Excitation::ExcitationFactory::create(const std::string & type)
{
  return mFactory.create(type);
}

bool Excitation::ExcitationFactory::registerType(const std::string &type, boost::function<Excitation *()> creator)
{
  mFactory.registerFactory(type, creator);
  return true;
}

void Excitation::excite(double freqGHz, double thetaInc, double phiInc, const mesh::Mesh *mesh, const MeshInformation *meshInfo, vector &rhs) const
{
  doExcite(freqGHz, thetaInc, phiInc, mesh, meshInfo, rhs);
}

void Excitation::print(std::ostream & output, int tabPos) const
{
  xmlutils::printValue(output, tabPos, "Name: ", mName);
  doPrint(output, tabPos);
}

void Excitation::print(int tabPos) const
{
  print(std::cout);
}

void Excitation::readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  setName(xmlutils::readAttribute<std::string>(node, "name"));
  doXmlRead(document, node);
}

void Excitation::writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "name", mName);
  doXmlWrite(document, node);
}

std::ostream & operator<<(std::ostream & output, const Excitation & object)
{
  object.print(output);
  return output;
}

}
