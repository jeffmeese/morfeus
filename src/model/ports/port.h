#ifndef MORFEUS_EXCITATIONS_PORT_H
#define MORFEUS_EXCITATIONS_PORT_H

#include "core/morfeusobject.h"

#include <string>

namespace morfeus {
namespace model {
namespace ports {

class Port
    : public core::MorfeusObject
{
protected:
  Port(const std::string & type);
  Port(const std::string & type, const std::string & name);
  Port(const std::string & type, const std::string & id, const std::string & name);
};

}
}
}

#endif // MORFEUS_EXCITATIONS_PORT_H
