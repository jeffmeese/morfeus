#include "medium.h"

namespace morfeus {
namespace model {
namespace media {

Medium::Medium(const std::string & type)
  : core::MorfeusObject (type)
{

}

Medium::Medium(const std::string & type, const std::string & name)
  : core::MorfeusObject (type, name)
{

}

Medium::Medium(const std::string & type, const std::string & id, const std::string & name)
  : core::MorfeusObject (type, id, name)
{

}

math::dcomplex Medium::value(double freqGHz) const
{
  return doValue(freqGHz);
}

}
}
}
