#ifndef ISOTROPICMATERIAL_H
#define ISOTROPICMATERIAL_H

#include "morfeus.h"

#include "material/material.h"

class IsotropicMaterial
    : public Material
{
public:
  MORFEUS_LIB_DECL IsotropicMaterial(std::size_t id);
  MORFEUS_LIB_DECL IsotropicMaterial(std::size_t id, const dcomplex & value);

public:
  MORFEUS_LIB_DECL dcomplex value() const;
  MORFEUS_LIB_DECL void setValue(const dcomplex &value);

public:
  MORFEUS_LIB_DECL virtual dcomplex value(Direction direction) const;

private:
  dcomplex mValue;
};

#endif // ISOTROPICMATERIAL_H
