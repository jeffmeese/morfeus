#include "layer.h"

Layer::Layer(std::size_t id, double thickness)
  : mId(id)
  , mThickness(thickness)
{
}

Layer::Layer(std::size_t id, double thickness, const std::string & name)
  : mId(id)
  , mThickness(thickness)
  , mName(name)
{
}

std::size_t Layer::id() const
{
  return mId;
}

std::string Layer::name() const
{
  return mName;
}

void Layer::setName(const std::string & name)
{
  mName = name;
}

void Layer::setThickness(double thickness)
{
  mThickness = thickness;
}

double Layer::thickness() const
{
  return mThickness;
}
