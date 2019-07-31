#ifndef MORFEUS_GEOMETRY_SURFACE_H
#define MORFEUS_GEOMETRY_SURFACE_H

#include "part.h"

namespace morfeus {
namespace geometry {

class Surface
    : public Part
{
protected:
  Surface(const std::string & type);
  Surface(const std::string & type, const std::string & name);
  Surface(const std::string & type, const std::string & id, const std::string & name);

protected:
  std::vector<Region*> doGetRegionList() const override;
};

}
}

#endif // MORFEUS_GEOMETRY_SURFACE_H
