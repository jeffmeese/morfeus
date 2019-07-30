#ifndef ISOTROPICMATERIAL_H
#define ISOTROPICMATERIAL_H

#include "material.h"

namespace morfeus {

class IsotropicMaterial
    : public Material
{
public:
  MORFEUS_LIB_DECL IsotropicMaterial();
  MORFEUS_LIB_DECL IsotropicMaterial(const std::string & name);
  MORFEUS_LIB_DECL IsotropicMaterial(const std::string & id, const std::string & name);
  MORFEUS_LIB_DECL IsotropicMaterial(const dcomplex & permittivity, const dcomplex & permeability);
  MORFEUS_LIB_DECL IsotropicMaterial(const std::string & namem, const dcomplex & permittivity, const dcomplex & permeability);
  MORFEUS_LIB_DECL IsotropicMaterial(const std::string & id, const std::string & name, const dcomplex & permittivity, const dcomplex & permeability);

public:
  MORFEUS_LIB_DECL dcomplex permeability() const;
  MORFEUS_LIB_DECL dcomplex permittivity() const;
  MORFEUS_LIB_DECL void setPermeability(dcomplex value);
  MORFEUS_LIB_DECL void setPermittivity(dcomplex value);

protected:
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  dcomplex mPermeability;
  dcomplex mPermittivity;
};

inline dcomplex IsotropicMaterial::permeability() const
{
  return mPermeability;
}

inline dcomplex IsotropicMaterial::permittivity() const
{
  return mPermittivity;
}

inline void IsotropicMaterial::setPermeability(dcomplex value)
{
  mPermeability = value;
}

inline void IsotropicMaterial::setPermittivity(dcomplex value)
{
  mPermittivity = value;
}

}

#endif // ISOTROPICMATERIAL_H
