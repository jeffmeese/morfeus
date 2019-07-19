#include "isotropicmaterial.h"

IsotropicMaterial::IsotropicMaterial(int32_t id)
  : Material (id)
  , mPermeability(0.0,0.0)
  , mPermittivity(0.0,0.0)
{
}

IsotropicMaterial::IsotropicMaterial(int32_t id, const dcomplex & permittivity, const dcomplex & permeability)
  : Material (id)
  , mPermeability(permeability)
  , mPermittivity(permittivity)
{

}
