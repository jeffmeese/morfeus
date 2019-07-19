#ifndef MATERIAL_H
#define MATERIAL_H

#include "morfeus.h"
#include "types.h"

#include <cstddef>

class Material
{
public:
  enum class Direction
  {
    xx, xy, xz,
    yx, yy, yz,
    zx, zy, zz
  };

public:
  MORFEUS_LIB_DECL Material(std::size_t id);

public:
  MORFEUS_LIB_DECL std::size_t id() const;

public:
  MORFEUS_LIB_DECL virtual dcomplex value(Direction direction) const = 0;

private:
  std::size_t mId;
};

#endif // MATERIAL_H
