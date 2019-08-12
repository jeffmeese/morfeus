#ifndef DIELECTRICMEDIUM_H
#define DIELECTRICMEDIUM_H

#include "medium.h"

namespace morfeus {
namespace model {
namespace media {

class Dielectric
    : public Medium
{
public:
  MORFEUS_LIB_DECL Dielectric();
  MORFEUS_LIB_DECL Dielectric(const std::string & name);
  MORFEUS_LIB_DECL Dielectric(const std::string & id, const std::string & name);
  MORFEUS_LIB_DECL Dielectric(math::dcomplex permittivity, math::dcomplex permeability);
  MORFEUS_LIB_DECL Dielectric(const std::string & name, math::dcomplex permittivity, math::dcomplex permeability);
  MORFEUS_LIB_DECL Dielectric(const std::string & id, const std::string & name, math::dcomplex permittivity, math::dcomplex permeability);

public:
  MORFEUS_LIB_DECL math::dcomplex permeability() const;
  MORFEUS_LIB_DECL math::dcomplex permittivity() const;
  MORFEUS_LIB_DECL void setPermeability(const math::dcomplex & value);
  MORFEUS_LIB_DECL void setPermittivity(const math::dcomplex & value);

protected:
  void doPrint(std::ostream & output, int tabPos) const override;
  math::dcomplex doValue(double freqGHz) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  math::dcomplex mPermeability;
  math::dcomplex mPermittivity;
};


inline math::dcomplex Dielectric::permeability() const
{
  return mPermeability;
}

inline math::dcomplex Dielectric::permittivity() const
{
  return mPermittivity;
}

inline void Dielectric::setPermeability(const math::dcomplex & value)
{
  mPermeability = value;
}

inline void Dielectric::setPermittivity(const math::dcomplex & value)
{
  mPermittivity = value;
}

}
}
}

#endif // DIELECTRICMEDIUM_H
