#include "medium.h"

namespace morfeus {
namespace media {

Medium::Medium(const std::string & type)
  : core::MorfeusObject (type)
{

}

Medium::Medium(const std::string & type, const std::string & name)
  : core::MorfeusObject (type)
  , mName(name)
{

}

Medium::Medium(const std::string & type, const std::string & id, const std::string & name)
  : core::MorfeusObject (type, id)
  , mName(name)
{

}

dcomplex Medium::value(double freqGHz) const
{
  return doValue(freqGHz);
}

}
}
