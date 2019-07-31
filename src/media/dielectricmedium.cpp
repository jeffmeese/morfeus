#include "dielectricmedium.h"

namespace morfeus {
namespace media {

static const std::string OBJECT_ID("DielectricMedium");

DielectricMedium::DielectricMedium()
  : Medium (OBJECT_ID)
{

}

DielectricMedium::DielectricMedium(const std::string & name)
  : Medium (OBJECT_ID, name)
{

}

DielectricMedium::DielectricMedium(const std::string & id, const std::string & name)
  : Medium (OBJECT_ID, id, name)
{
}

DielectricMedium::DielectricMedium(dcomplex permittivity, dcomplex permeability)
  : Medium (OBJECT_ID)
  , mPermeability(permeability)
  , mPermittivity(permittivity)
{

}

DielectricMedium::DielectricMedium(const std::string & name, dcomplex permittivity, dcomplex permeability)
  : Medium (OBJECT_ID, name)
  , mPermeability(permeability)
  , mPermittivity(permittivity)
{

}

DielectricMedium::DielectricMedium(const std::string & id, const std::string & name, dcomplex permittivity, dcomplex permeability)
  : Medium (OBJECT_ID, id, name)
  , mPermeability(permeability)
  , mPermittivity(permittivity)
{
}

void DielectricMedium::doPrint(std::ostream & output, int tabPos) const
{

}

dcomplex DielectricMedium::doValue(double freqGHz) const
{
  dcomplex value(0.0,0.0);
  return value;
}

void DielectricMedium::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{

}

void DielectricMedium::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{

}

}
}
