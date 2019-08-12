#ifndef MORFEUS_MODEL_GEOMETRY_PART_H
#define MORFEUS_MODEL_GEOMETRY_PART_H

#include "core/factory.h"
#include "core/morfeusobject.h"

#include <vector>

namespace morfeus {
  namespace model {
    namespace geometry {
      class Face;
      class Point;
      class Region;
      class Segment;
      class Vertex;
      class Wire;
    }
  }
}

namespace morfeus {
namespace model {
namespace geometry {

class Part
    : public core::MorfeusObject
{
  class PartFactory;

public:
  MORFEUS_LIB_DECL bool containsPoint(const Point & pt) const;
  MORFEUS_LIB_DECL std::vector<Face*> getFaceList() const;
  MORFEUS_LIB_DECL std::vector<Region*> getRegionList() const;
  MORFEUS_LIB_DECL std::vector<Segment*> getSegmentList() const;
  MORFEUS_LIB_DECL std::vector<Vertex*> getVertexList() const;
  MORFEUS_LIB_DECL std::vector<Wire*> getWireList() const;

public:
  MORFEUS_LIB_DECL static PartFactory & factory();

protected:
  Part(const std::string & type);
  Part(const std::string & type, const std::string & name);
  Part(const std::string & type, const std::string & id, const std::string & name);

protected:
  virtual bool doContainsPoint(const Point & pt) const = 0;
  virtual std::vector<Face*> doGetFaceList() const = 0;
  virtual std::vector<Region*> doGetRegionList() const = 0;
  virtual std::vector<Segment*> doGetSegmentList() const = 0;
  virtual std::vector<Vertex*> doGetVertexList() const = 0;
  virtual std::vector<Wire*> doGetWireList() const = 0;

private:
  class PartFactory
  {
  public:
    MORFEUS_LIB_DECL Part * create(const std::string & type);
    MORFEUS_LIB_DECL bool registerType(const std::string & type, boost::function<Part*()> creator);

  private:
    core::Factory<Part*, std::string, boost::function<Part*()> > mFactory;
  };
};

}
}
}

#endif // MORFEUS_MODEL_GEOMETRY_PART_H
