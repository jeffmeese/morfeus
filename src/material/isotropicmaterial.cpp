#include "isotropicmaterial.h"

#include "xmlutils.h"

static const std::string OBJECT_ID("Isotropic Material");

IsotropicMaterial::IsotropicMaterial()
  : mPermeability(0.0,0.0)
  , mPermittivity(0.0,0.0)
{
}

IsotropicMaterial::IsotropicMaterial(int32_t number, const dcomplex & permittivity, const dcomplex & permeability)
  : Material (number)
  , mPermeability(permeability)
  , mPermittivity(permittivity)
{
}

void IsotropicMaterial::doPrint(std::ostream & output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
  xmlutils::printValue(output, tabPos+2, "Permittivity", mPermittivity);
  xmlutils::printValue(output, tabPos+2, "Permeability", mPermeability);
}

void IsotropicMaterial::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  setName(xmlutils::readAttribute<std::string>(node, "name"));
  setNumber(std::stoi(xmlutils::readAttribute<std::string>(node, "number")));
  double eps_r = std::stod(xmlutils::readAttribute<std::string>(node, "eps_r"));
  double eps_i = std::stod(xmlutils::readAttribute<std::string>(node, "eps_i"));
  double mu_r = std::stod(xmlutils::readAttribute<std::string>(node, "mu_r"));
  double mu_i = std::stod(xmlutils::readAttribute<std::string>(node, "mu_i"));
  setPermittivity(dcomplex(eps_r, eps_i));
  setPermeability(dcomplex(mu_r, mu_i));
}

void IsotropicMaterial::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "name", name());
  xmlutils::writeAttribute(document, node, "number", number());
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, node, "eps_r", mPermittivity.real());
  xmlutils::writeAttribute(document, node, "eps_i", mPermittivity.imag());
  xmlutils::writeAttribute(document, node, "mu_r", mPermeability.real());
  xmlutils::writeAttribute(document, node, "mu_i", mPermeability.imag());
}

namespace  {
  Material * createFunc()
  {
    return new IsotropicMaterial;
  }

  const bool registered = Material::Factory::Instance().Register(OBJECT_ID, createFunc);
}
