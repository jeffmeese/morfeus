#include "wireport.h"

namespace morfeus {
namespace model {
namespace ports {

static const std::string OBJECT_ID("WirePort");

WirePort::WirePort()
  : Port(OBJECT_ID)
{

}

WirePort::WirePort(const std::string & name)
  : Port(OBJECT_ID, name)
{

}

WirePort::WirePort(const std::string & id, const std::string & name)
  : Port(OBJECT_ID, id, name)
{

}

}
}
}
