#include "material.h"

Material::Material()
{
  init();
}

Material::Material(int32_t number)
  : MorfeusObject (number)
{
  init();
}

Material::Material(const std::string & name, int32_t number)
  : MorfeusObject(name, number)
{
  init();
}

void Material::init()
{
  for (std::size_t i = 0; i < 9; i++) {
    mValues[i] = dcomplex(1.0, 0.0);
  }
}
