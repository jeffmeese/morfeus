#ifndef ISOTROPICMATERIAL_H
#define ISOTROPICMATERIAL_H

#include "material.h"

class IsotropicMaterial
    : public Material
{
public:
  MORFEUS_LIB_DECL IsotropicMaterial(int32_t id);
  MORFEUS_LIB_DECL IsotropicMaterial(int32_t id, const dcomplex & permittivity, const dcomplex & permeability);

public:
  MORFEUS_LIB_DECL dcomplex permeability() const;
  MORFEUS_LIB_DECL dcomplex permittivity() const;
  MORFEUS_LIB_DECL void setPermeability(dcomplex value);
  MORFEUS_LIB_DECL void setPermittivity(dcomplex value);

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

#endif // ISOTROPICMATERIAL_H
