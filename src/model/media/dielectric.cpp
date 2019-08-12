#include "dielectric.h"

namespace morfeus {
namespace model {
namespace media {

static const std::string OBJECT_ID("DielectricMedium");

Dielectric::Dielectric()
  : Medium (OBJECT_ID)
{
  setPermeability(math::dcomplex(1.0,0.0));
  setPermittivity(math::dcomplex(1.0,0.0));
}

Dielectric::Dielectric(const std::string & name)
  : Medium (OBJECT_ID, name)
{
  setPermeability(math::dcomplex(1.0,0.0));
  setPermittivity(math::dcomplex(1.0,0.0));
}

Dielectric::Dielectric(const std::string & id, const std::string & name)
  : Medium (OBJECT_ID, id, name)
{
  setPermeability(math::dcomplex(1.0,0.0));
  setPermittivity(math::dcomplex(1.0,0.0));
}

Dielectric::Dielectric(math::dcomplex permittivity, math::dcomplex permeability)
  : Medium (OBJECT_ID)
  , mPermeability(permeability)
  , mPermittivity(permittivity)
{

}

Dielectric::Dielectric(const std::string & name, math::dcomplex permittivity, math::dcomplex permeability)
  : Medium (OBJECT_ID, name)
  , mPermeability(permeability)
  , mPermittivity(permittivity)
{

}

Dielectric::Dielectric(const std::string & id, const std::string & name, math::dcomplex permittivity, math::dcomplex permeability)
  : Medium (OBJECT_ID, id, name)
  , mPermeability(permeability)
  , mPermittivity(permittivity)
{
}

void Dielectric::doPrint(std::ostream & output, int tabPos) const
{

}

math::dcomplex Dielectric::doValue(double freqGHz) const
{
  math::dcomplex value(0.0,0.0);
  return value;
}

void Dielectric::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{

}

void Dielectric::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{

}

}
}
}
