#include "solid.h"

namespace morfeus {
namespace geometry {

Solid::Solid(const std::string & type)
  : Part(type)
{

}

Solid::Solid(const std::string & type, const std::string & name)
  : Part(type)
{

}

Solid::Solid(const std::string & type, const std::string & id, const std::string & name)
  : Part(type, id)
{

}

}
}
