#ifndef MORFEUS_GEOMETRY_PART_H
#define MORFEUS_GEOMETRY_PART_H

#include "core/factory.h"
#include "core/morfeusobject.h"

#include <vector>

namespace morfeus {
  namespace geometry {
    class Face;
    class Region;
    class Segment;
  }
}

namespace morfeus {
namespace geometry {

class Part
    : public core::MorfeusObject
{
  class PartFactory;

public:
  MORFEUS_LIB_DECL std::string name() const;
  MORFEUS_LIB_DECL void setName(const std::string & name);

public:
  MORFEUS_LIB_DECL std::vector<Face*> getFaceList() const;
  MORFEUS_LIB_DECL std::vector<Region*> getRegionList() const;
  MORFEUS_LIB_DECL std::vector<Segment*> getSegmentList() const;

public:
  MORFEUS_LIB_DECL static PartFactory & factory();

protected:
  virtual std::vector<Face*> doGetFaceList() const = 0;
  virtual std::vector<Region*> doGetRegionList() const = 0;
  virtual std::vector<Segment*> doGetSegmentList() const = 0;

private:
  class PartFactory
  {
  public:
    MORFEUS_LIB_DECL Part * create(const std::string & type);
    MORFEUS_LIB_DECL bool registerType(const std::string & type, boost::function<Part*()> creator);

  private:
    core::Factory<Part*, std::string, boost::function<Part*()> > mFactory;
  };

protected:
  Part(const std::string & type);
  Part(const std::string & type, const std::string & id);

private:
  std::string mName;
};

inline std::string Part::name() const
{
  return mName;
}

inline void Part::setName(const std::string & name)
{
  mName = name;
}

}
}

#endif // MORFEUS_GEOMETRY_PART_H
