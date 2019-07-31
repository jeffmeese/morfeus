#include "part.h"

namespace morfeus {
namespace geometry {

Part::Part(const std::string & type)
  : core::MorfeusObject (type)
{
}

Part::Part(const std::string & type, const std::string & id)
  : core::MorfeusObject (type, id)
{

}

std::vector<Face*> Part::getFaceList() const
{
  return doGetFaceList();
}

std::vector<Region*> Part::getRegionList() const
{
  return doGetRegionList();
}

std::vector<Segment*> Part::getSegmentList() const
{
  return doGetSegmentList();
}

Part::PartFactory & Part::factory()
{
  static PartFactory f;
  return f;
}

bool Part::PartFactory::registerType(const std::string &type, boost::function<Part *()> creator)
{
  mFactory.registerFactory(type, creator);
  return true;
}

Part * Part::PartFactory::create(const std::string & type)
{
  return mFactory.create(type);
}

}
}
