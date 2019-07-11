#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "morfeus.h"
#include "types.h"

#include <memory>
#include <vector>

class Hole;
class Mesh;
class Region;
class Segment;
class Vertex;

class Geometry
{
public:
  MORFEUS_LIB_DECL Geometry();
  MORFEUS_LIB_DECL virtual ~Geometry();

public:
  MORFEUS_LIB_DECL const Hole * hole(int32_t index) const;
  MORFEUS_LIB_DECL Hole * hole(int32_t index);
  MORFEUS_LIB_DECL const Region * region(int32_t index) const;
  MORFEUS_LIB_DECL Region * region(int32_t index);
  MORFEUS_LIB_DECL const Segment * segment(int32_t index) const;
  MORFEUS_LIB_DECL Segment * segment(int32_t index);
  MORFEUS_LIB_DECL const Vertex * vertex(int32_t index) const;
  MORFEUS_LIB_DECL Vertex * vertex(int32_t index);
  MORFEUS_LIB_DECL int32_t totalHoles() const;
  MORFEUS_LIB_DECL int32_t totalRegions() const;
  MORFEUS_LIB_DECL int32_t totalSegments() const;
  MORFEUS_LIB_DECL int32_t totalVertices() const;

public:
  MORFEUS_LIB_DECL void addHole(std::unique_ptr<Hole> node);
  MORFEUS_LIB_DECL void addRegion(std::unique_ptr<Region> region);
  MORFEUS_LIB_DECL void addSegment(std::unique_ptr<Segment> node);
  MORFEUS_LIB_DECL void addVertex(std::unique_ptr<Vertex> node);
  MORFEUS_LIB_DECL void createMesh(Mesh & mesh);
  MORFEUS_LIB_DECL void readPolyFile(const std::string & fileName);

private:
  void readPolyHoles(std::ifstream & inputFile);
  void readPolyRegions(std::ifstream & inputFile, int32_t numRegions);
  void readPolySegments(std::ifstream & inputFile);
  int32_t readPolyVertices(std::ifstream & inputFile);

private:
  typedef std::unique_ptr<Hole> HolePtr;
  typedef std::unique_ptr<Region> RegionPtr;
  typedef std::unique_ptr<Segment> SegmentPtr;
  typedef std::unique_ptr<Vertex> VertexPtr;
  typedef std::vector<HolePtr> HoleVector;
  typedef std::vector<RegionPtr> RegionVector;
  typedef std::vector<SegmentPtr> SegmentVector;
  typedef std::vector<VertexPtr> VertexVector;

private:
  double mMaxArea;
  double mMinArea;
  HoleVector mHoles;
  RegionVector mRegions;
  SegmentVector mSegments;
  VertexVector mVertices;
};

inline const Hole * Geometry::hole(int32_t index) const
{
  return mHoles.at(index).get();
}

inline Hole * Geometry::hole(int32_t index)
{
  return mHoles.at(index).get();
}

inline const Region * Geometry::region(int32_t index) const
{
  return mRegions.at(index).get();
}

inline Region * Geometry::region(int32_t index)
{
  return mRegions.at(index).get();
}

inline const Segment * Geometry::segment(int32_t index) const
{
  return mSegments.at(index).get();
}

inline Segment * Geometry::segment(int32_t index)
{
  return mSegments.at(index).get();
}

inline const Vertex * Geometry::vertex(int32_t index) const
{
  return mVertices.at(index).get();
}

inline Vertex * Geometry::vertex(int32_t index)
{
  return mVertices.at(index).get();
}

inline int32_t Geometry::totalHoles() const
{
  return static_cast<int32_t>(mHoles.size());
}

inline int32_t Geometry::totalRegions() const
{
  return static_cast<int32_t>(mRegions.size());
}

inline int32_t Geometry::totalSegments() const
{
  return static_cast<int32_t>(mSegments.size());
}

inline int32_t Geometry::totalVertices() const
{
  return static_cast<int32_t>(mVertices.size());
}

#endif // GEOMETRY_H
