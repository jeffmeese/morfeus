#ifndef MORFEUS_MODEL_GEOMETRY_POLYGON_H
#define MORFEUS_MODEL_GEOMETRY_POLYGON_H

#include "part.h"

#include "math/types.h"

namespace morfeus {
  namespace model {
    namespace geometry {
      class Facet;
      class Point;
      class Vertex;
    }
  }
}

namespace morfeus {
namespace model {
namespace geometry {

class Polygon
    : public Part
{
public:
  MORFEUS_LIB_DECL Polygon();
  MORFEUS_LIB_DECL Polygon(const std::string & name);
  MORFEUS_LIB_DECL Polygon(const std::string & id, const std::string & name);

public:
  MORFEUS_LIB_DECL int32_t number() const;
  MORFEUS_LIB_DECL Point * point(std::size_t index);
  MORFEUS_LIB_DECL const Point * point(std::size_t index) const;
  MORFEUS_LIB_DECL std::size_t totalPoints() const;

public:
  MORFEUS_LIB_DECL void addPoint(std::unique_ptr<Point> point);
  MORFEUS_LIB_DECL math::vec3d computeNormal() const;

public:
  MORFEUS_LIB_DECL static int32_t totalCreated();

protected:
  bool doContainsPoint(const Point & pt) const override;
  std::vector<Face*> doGetFaceList() const override;
  std::vector<Region*> doGetRegionList() const override;
  std::vector<Segment*> doGetSegmentList() const override;
  std::vector<Vertex*> doGetVertexList() const override;
  std::vector<Wire*> doGetWireList() const override;
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  void init();

private:
  typedef std::unique_ptr<Face> FacePtr;
  typedef std::unique_ptr<Point> PointPtr;
  typedef std::unique_ptr<Segment> SegmentPtr;
  typedef std::unique_ptr<Vertex> VertexPtr;
  typedef std::vector<PointPtr> PointVector;
  typedef std::vector<SegmentPtr> SegmentVector;
  typedef std::vector<VertexPtr> VertexVector;

private:
  static int32_t mTotal;
  int32_t mNumber;
  FacePtr mFace;
  PointVector mPoints;
  SegmentVector mSegments;
  VertexVector mVertices;
};

inline int32_t Polygon::number() const
{
  return mNumber;
}

inline const Point * Polygon::point(std::size_t index) const
{
  return mPoints.at(index).get();
}

inline Point * Polygon::point(std::size_t index)
{
  return mPoints.at(index).get();
}

inline std::size_t Polygon::totalPoints() const
{
  return mPoints.size();
}

}
}
}

#endif // MORFEUS_MODEL_GEOMETRY_POLYGON_H
