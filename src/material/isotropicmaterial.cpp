#include "isotropicmaterial.h"

#include "xmlutils.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

static const std::string OBJECT_ID("Isotropic Material");

IsotropicMaterial::IsotropicMaterial()
  : Material(OBJECT_ID)
  , mPermeability(0.0,0.0)
  , mPermittivity(0.0,0.0)
{
}

IsotropicMaterial::IsotropicMaterial(const std::string & name)
  : Material(OBJECT_ID, name)
  , mPermeability(0.0,0.0)
  , mPermittivity(0.0,0.0)
{
}

IsotropicMaterial::IsotropicMaterial(const std::string & id, const std::string & name)
  : Material(OBJECT_ID, id, name)
  , mPermeability(0.0,0.0)
  , mPermittivity(0.0,0.0)
{
}

IsotropicMaterial::IsotropicMaterial(const dcomplex & permittivity, const dcomplex & permeability)
  : Material (OBJECT_ID)
  , mPermeability(permeability)
  , mPermittivity(permittivity)
{
}

IsotropicMaterial::IsotropicMaterial(const std::string & name, const dcomplex & permittivity, const dcomplex & permeability)
  : Material (OBJECT_ID, name)
  , mPermeability(permeability)
  , mPermittivity(permittivity)
{
}

IsotropicMaterial::IsotropicMaterial(const std::string & id, const std::string & name, const dcomplex & permittivity, const dcomplex & permeability)
  : Material (OBJECT_ID, id, name)
  , mPermeability(permeability)
  , mPermittivity(permittivity)
{
}

void IsotropicMaterial::doPrint(std::ostream & output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
  xmlutils::printValue(output, tabPos, "Permittivity", mPermittivity);
  xmlutils::printValue(output, tabPos, "Permeability", mPermeability);
}

void IsotropicMaterial::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  double eps_r = xmlutils::readAttribute<double>(node, "eps_r");
  double eps_i = xmlutils::readAttribute<double>(node, "eps_i");
  double mu_r = xmlutils::readAttribute<double>(node, "mu_r");
  double mu_i = xmlutils::readAttribute<double>(node, "mu_i");
  setPermittivity(dcomplex(eps_r, eps_i));
  setPermeability(dcomplex(mu_r, mu_i));
}

void IsotropicMaterial::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, node, "eps_r", mPermittivity.real());
  xmlutils::writeAttribute(document, node, "eps_i", mPermittivity.imag());
  xmlutils::writeAttribute(document, node, "mu_r", mPermeability.real());
  xmlutils::writeAttribute(document, node, "mu_i", mPermeability.imag());
}

namespace  {
  const bool r = Material::factory().registerType(OBJECT_ID,boost::bind(boost::factory<IsotropicMaterial*>()));
}

