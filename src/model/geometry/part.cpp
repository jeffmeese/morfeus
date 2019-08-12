#include "part.h"

namespace morfeus {
namespace model {
namespace geometry {

Part::Part(const std::string & type)
  : core::MorfeusObject (type)
{

}

Part::Part(const std::string & type, const std::string & name)
  : core::MorfeusObject (type, name)
{

}

Part::Part(const std::string & type, const std::string & id, const std::string & name)
  : core::MorfeusObject (type, id, name)
{

}

bool Part::containsPoint(const Point & pt) const
{
  return doContainsPoint(pt);
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

std::vector<Vertex*> Part::getVertexList() const
{
  return doGetVertexList();
}

std::vector<Wire*> Part::getWireList() const
{
  return doGetWireList();
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
}
