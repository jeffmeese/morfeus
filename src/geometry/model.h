#ifndef MORFEUS_GEOMETRY_MODEL_H
#define MORFEUS_GEOMETRY_MODEL_H

#include "morfeus.h"
#include "morfeusobject.h"

#include "rapidxml.hpp"
#include "xmlutils.h"

#include <memory>
#include <vector>

namespace morfeus {
namespace geometry {

class Hole;
class Mesh;
class Region;
class Segment;
class Shape;
class Vertex;

class Model
    : public MorfeusObject
{
public:
  MORFEUS_LIB_DECL Model();
  MORFEUS_LIB_DECL virtual ~Model();

public:
  MORFEUS_LIB_DECL const Hole * hole(std::size_t index) const;
  MORFEUS_LIB_DECL Hole * hole(std::size_t index);
  MORFEUS_LIB_DECL const Region * region(std::size_t index) const;
  MORFEUS_LIB_DECL Region * region(std::size_t index);
  MORFEUS_LIB_DECL Shape * shape(std::size_t index);
  MORFEUS_LIB_DECL const Shape * shape(std::size_t index) const;
  MORFEUS_LIB_DECL std::size_t totalHoles() const;
  MORFEUS_LIB_DECL std::size_t totalRegions() const;
  MORFEUS_LIB_DECL std::size_t totalShapes() const;

public:
  MORFEUS_LIB_DECL void addHole(std::unique_ptr<Hole> region);
  MORFEUS_LIB_DECL void addRegion(std::unique_ptr<Region> region);
  MORFEUS_LIB_DECL void addShape(std::unique_ptr<Shape> shape);
  MORFEUS_LIB_DECL void createMesh(Mesh & mesh);
  MORFEUS_LIB_DECL void print(std::ostream & output, int tabPos = 0) const;
  MORFEUS_LIB_DECL void print(int tabPos = 0) const;
  MORFEUS_LIB_DECL void readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node);
  MORFEUS_LIB_DECL void writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const;
  MORFEUS_LIB_DECL friend std::ostream & operator<<(std::ostream & output, const Model & object);

private:
  typedef std::unique_ptr<Hole> HolePtr;
  typedef std::unique_ptr<Region> RegionPtr;
  typedef std::unique_ptr<Shape> ShapePtr;
  typedef std::vector<HolePtr> HoleVector;
  typedef std::vector<RegionPtr> RegionVector;
  typedef std::vector<ShapePtr> ShapeVector;

private:
  double mMaxArea;
  double mMinArea;
  HoleVector mHoles;
  RegionVector mRegions;
  ShapeVector mShapes;
};

inline const Hole * Model::hole(std::size_t index) const
{
  return mHoles.at(index).get();
}

inline Hole * Model::hole(std::size_t index)
{
  return mHoles.at(index).get();
}

inline const Region * Model::region(std::size_t index) const
{
  return mRegions.at(index).get();
}

inline Region * Model::region(std::size_t index)
{
  return mRegions.at(index).get();
}

inline const Shape * Model::shape(std::size_t index) const
{
  return mShapes.at(index).get();
}

inline Shape * Model::shape(std::size_t index)
{
  return mShapes.at(index).get();
}

inline std::size_t Model::totalHoles() const
{
  return mHoles.size();
}

inline std::size_t Model::totalRegions() const
{
  return mRegions.size();
}

inline std::size_t Model::totalShapes() const
{
  return mShapes.size();
}

}
}

#endif // MORFEUS_GEOMETRY_MODEL_H
