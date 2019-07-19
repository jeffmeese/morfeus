#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "morfeus.h"
#include "morfeusobject.h"

#include <memory>
#include <vector>

class Hole;
class Mesh;
class Region;
class Segment;
class Shape;
class Vertex;

class Geometry
    : public MorfeusObject
{
public:
  MORFEUS_LIB_DECL Geometry();
  MORFEUS_LIB_DECL virtual ~Geometry();

public:
  MORFEUS_LIB_DECL const Region * region(std::size_t index) const;
  MORFEUS_LIB_DECL Region * region(std::size_t index);
  MORFEUS_LIB_DECL const Segment * segment(std::size_t index) const;
  MORFEUS_LIB_DECL Segment * segment(std::size_t index);
  MORFEUS_LIB_DECL Shape * shape(std::size_t index);
  MORFEUS_LIB_DECL const Shape * shape(std::size_t index) const;
  MORFEUS_LIB_DECL Vertex * vertex(std::size_t index);
  MORFEUS_LIB_DECL const Vertex * vertex(std::size_t index) const;
  MORFEUS_LIB_DECL std::size_t totalRegions() const;
  MORFEUS_LIB_DECL std::size_t totalSegments() const;
  MORFEUS_LIB_DECL std::size_t totalShapes() const;
  MORFEUS_LIB_DECL std::size_t totalVertices() const;

public:
  MORFEUS_LIB_DECL void addRegion(std::unique_ptr<Region> region);
  MORFEUS_LIB_DECL void addSegment(std::unique_ptr<Segment> node);
  MORFEUS_LIB_DECL void addShape(std::unique_ptr<Shape> shape);
  MORFEUS_LIB_DECL void addVertex(std::unique_ptr<Vertex> node);
  MORFEUS_LIB_DECL void createMesh(Mesh & mesh);
  MORFEUS_LIB_DECL void readPolyFile(const std::string & fileName);

protected:
  void doReadFromXml(ptree & tree) override;
  void doWriteToXml(ptree & tree) const override;

private:
  void readPolyHoles(std::ifstream & inputFile);
  void readPolyRegions(std::ifstream & inputFile, int32_t numRegions);
  void readPolySegments(std::ifstream & inputFile);
  int32_t readPolyVertices(std::ifstream & inputFile);

private:
  typedef std::unique_ptr<Region> RegionPtr;
  typedef std::unique_ptr<Segment> SegmentPtr;
  typedef std::unique_ptr<Shape> ShapePtr;
  typedef std::unique_ptr<Vertex> VertexPtr;
  typedef std::vector<RegionPtr> RegionVector;
  typedef std::vector<SegmentPtr> SegmentVector;
  typedef std::vector<ShapePtr> ShapeVector;
  typedef std::vector<VertexPtr> VertexVector;

private:
  double mMaxArea;
  double mMinArea;
  RegionVector mRegions;
  SegmentVector mSegments;
  ShapeVector mShapes;
  VertexVector mVertices;
};

inline const Region * Geometry::region(std::size_t index) const
{
  return mRegions.at(index).get();
}

inline Region * Geometry::region(std::size_t index)
{
  return mRegions.at(index).get();
}

inline const Segment * Geometry::segment(std::size_t index) const
{
  return mSegments.at(index).get();
}

inline Segment * Geometry::segment(std::size_t index)
{
  return mSegments.at(index).get();
}

inline const Shape * Geometry::shape(std::size_t index) const
{
  return mShapes.at(index).get();
}

inline Shape * Geometry::shape(std::size_t index)
{
  return mShapes.at(index).get();
}

inline const Vertex * Geometry::vertex(std::size_t index) const
{
  return mVertices.at(index).get();
}

inline Vertex * Geometry::vertex(std::size_t index)
{
  return mVertices.at(index).get();
}

inline std::size_t Geometry::totalRegions() const
{
  return static_cast<int32_t>(mRegions.size());
}

inline std::size_t Geometry::totalSegments() const
{
  return static_cast<int32_t>(mSegments.size());
}

inline std::size_t Geometry::totalShapes() const
{
  return mShapes.size();
}

inline std::size_t Geometry::totalVertices() const
{
  return static_cast<int32_t>(mVertices.size());
}

#endif // GEOMETRY_H
