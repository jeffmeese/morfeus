#include "surface.h"

namespace morfeus {
namespace geometry {

Surface::Surface(const std::string & type)
  : Part(type)
{

}

Surface::Surface(const std::string & type, const std::string & name)
  : Part(type)
{

}

Surface::Surface(const std::string & type, const std::string & id, const std::string & name)
  : Part(type, id)
{

}

std::vector<Region*> Surface::doGetRegionList() const
{
  std::vector<Region*> regionList;
  return regionList;
}

}
}
