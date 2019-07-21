#ifndef MATERIAL_H
#define MATERIAL_H

#include "morfeus.h"
#include "morfeusobject.h"

#include <map>

#include <Factory.h>
#include <Singleton.h>

class Material
    : public MorfeusObject
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
  MORFEUS_LIB_DECL Material();
  MORFEUS_LIB_DECL Material(int32_t number);
  MORFEUS_LIB_DECL Material(const std::string & name, int32_t number);

public:
  MORFEUS_LIB_DECL dcomplex value(Direction direction) const;
  MORFEUS_LIB_DECL void setValue(Direction direction, dcomplex value);

public:
  typedef Loki::SingletonHolder<Loki::Factory<Material, std::string> > Factory;

private:
  void init();

private:
  dcomplex mValues[9];
};

inline void Material::setValue(Direction direction, dcomplex value)
{
  mValues[direction] = value;
}

inline dcomplex Material::value(Direction direction) const
{
  return mValues[direction];
}

#endif // MATERIAL_H
