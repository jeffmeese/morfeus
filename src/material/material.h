#ifndef MATERIAL_H
#define MATERIAL_H

#include "morfeus.h"

#include <map>

class Material
{
public:
  enum Direction
  {
    xx = 0,
    xy,
    xz,
    yx,
    yy,
    yz,
    zx,
    zy,
    zz
  };

public:
  MORFEUS_LIB_DECL Material(int32_t id);

public:
  MORFEUS_LIB_DECL int32_t id() const;
  MORFEUS_LIB_DECL dcomplex value(Direction direction) const;
  MORFEUS_LIB_DECL void setValue(Direction direction, dcomplex value);

private:
  int32_t mId;
  dcomplex mValues[9];
};

inline int32_t Material::id() const
{
  return mId;
}

inline void Material::setValue(Direction direction, dcomplex value)
{
  mValues[direction] = value;
}

inline dcomplex Material::value(Direction direction) const
{
  return mValues[direction];
}

#endif // MATERIAL_H
