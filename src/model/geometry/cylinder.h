#ifndef MORFEUS_MODEL_GEOMETRY_CYLINDER_H
#define MORFEUS_MODEL_GEOMETRY_CYLINDER_H

#include "part.h"
#include "point.h"

namespace morfeus {
namespace model {
namespace geometry {

class Cylinder
    : public Part
{
public:
  MORFEUS_LIB_DECL Cylinder();
  MORFEUS_LIB_DECL Cylinder(const std::string & name);
  MORFEUS_LIB_DECL Cylinder(const std::string & id, const std::string & name);
  MORFEUS_LIB_DECL Cylinder(const Point & center, double radius, double height);
  MORFEUS_LIB_DECL Cylinder(const std::string & name, const Point & center, double radius, double height);
  MORFEUS_LIB_DECL Cylinder(const std::string & id, const std::string & name, const Point & center, double radius, double height);

public:
  MORFEUS_LIB_DECL Point center() const;
  MORFEUS_LIB_DECL double height() const;
  MORFEUS_LIB_DECL int32_t number() const;
  MORFEUS_LIB_DECL double radius() const;
  MORFEUS_LIB_DECL std::size_t resolution() const;
  MORFEUS_LIB_DECL void setCenter(const Point & pt);
  MORFEUS_LIB_DECL void setHeight(double value);
  MORFEUS_LIB_DECL void setRadius(double value);
  MORFEUS_LIB_DECL void setResolution(std::size_t value);

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
  void init(const Point & center, double radius, double height);
  void updatePosition();

private:
  typedef std::unique_ptr<Face> FacePtr;
  typedef std::unique_ptr<Region> RegionPtr;
  typedef std::unique_ptr<Segment> SegmentPtr;
  typedef std::unique_ptr<Vertex> VertexPtr;
  typedef std::vector<FacePtr> FaceList;
  typedef std::vector<SegmentPtr> SegmentList;
  typedef std::vector<VertexPtr> VertexList;

private:
  static int mTotal;
  int32_t mNumber;
  std::size_t mResolution;
  double mHeight;
  double mRadius;
  Point mCenter;
  FaceList mFaceList;
  RegionPtr mRegion;
  SegmentList mSegmentList;
  VertexList mVertexList;
};

inline Point Cylinder::center() const
{
  return mCenter;
}


inline double Cylinder::height() const
{
  return mHeight;
}

inline int32_t Cylinder::number() const
{
  return mNumber;
}

inline double Cylinder::radius() const
{
  return mRadius;
}

inline std::size_t Cylinder::resolution() const
{
  return mResolution;
}

inline int32_t Cylinder::totalCreated()
{
  return mTotal;
}

}
}
}

#endif // MORFEUS_MODEL_GEOMETRY_CYLINDER_H
