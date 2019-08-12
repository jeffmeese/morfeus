#include "port.h"

namespace morfeus {
namespace model {
namespace ports {

Port::Port(const std::string & type)
  : core::MorfeusObject (type)
{

}

Port::Port(const std::string & type, const std::string & name)
  : core::MorfeusObject (type, name)
{

}

Port::Port(const std::string & type, const std::string & id, const std::string & name)
  : core::MorfeusObject (type, id, name)
{
}

}
}
}
