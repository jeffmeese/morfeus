#include "material/material.h"

Material::Material(std::size_t id)
  : mId(id)
{
}

std::size_t Material::id() const
{
  return mId;
}
