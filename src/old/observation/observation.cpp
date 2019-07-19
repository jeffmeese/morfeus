#include "observation.h"

Observation::Observation(std::size_t id, const std::string & name)
  : mId(id)
  , mName(name)
{
}

std::size_t Observation::id() const
{
  return mId;
}

std::string Observation::name() const
{
  return mName;
}

void Observation::setName(const std::string &name)
{
  mName = name;
}
