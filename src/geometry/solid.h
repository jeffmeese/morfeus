#ifndef MORFEUS_GEOMETRY_SOLID_H
#define MORFEUS_GEOMETRY_SOLID_H

#include "part.h"

namespace morfeus {
namespace geometry {

class Solid
    : public Part
{
protected:
  Solid(const std::string & type);
  Solid(const std::string & type, const std::string & name);
  Solid(const std::string & type, const std::string & id, const std::string & name);
};

}
}

#endif // MORFEUS_GEOMETRY_SOLID_H
