#include "material.h"

Material::Material(int32_t id)
  : mId(id)
{
  for (std::size_t i = 0; i < 9; i++) {
    mValues[i] = dcomplex(1.0, 0.0);
  }
}
