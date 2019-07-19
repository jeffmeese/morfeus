#include "material/isotropicmaterial.h"

IsotropicMaterial::IsotropicMaterial(std::size_t id)
  : Material(id)
{
  setValue(dcomplex(1.0, 0.0));
}

IsotropicMaterial::IsotropicMaterial(std::size_t id, const dcomplex & value)
  : Material(id)
{
  setValue(value);
}

void IsotropicMaterial::setValue(const dcomplex &value)
{
  mValue = value;
}

dcomplex IsotropicMaterial::value(Direction) const
{
  return mValue;
}

dcomplex IsotropicMaterial::value() const
{
  return mValue;
}
