#include "excitation/excitation.h"

Excitation::Excitation(std::size_t id)
  : mId(id)
{
}

std::size_t Excitation::id() const
{
  return mId;
}
