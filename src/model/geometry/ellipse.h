#ifndef MORFEUS_MODEL_GEOMETRY_ELLIPSE_H
#define MORFEUS_MODEL_GEOMETRY_ELLIPSE_H

#include "part.h"
#include "point.h"

namespace morfeus {
namespace model {
namespace geometry {

class Ellipse
    : public Part
{
public:
  MORFEUS_LIB_DECL Ellipse();
  MORFEUS_LIB_DECL Ellipse(const std::string & name);
  MORFEUS_LIB_DECL Ellipse(const std::string & id, const std::string & name);
  MORFEUS_LIB_DECL Ellipse(const Point & pt, double uRadius, double vRadius);
  MORFEUS_LIB_DECL Ellipse(const std::string & name, const Point & pt, double uRadius, double vRadius);
  MORFEUS_LIB_DECL Ellipse(const std::string & id, const std::string & name, const Point & pt, double uRadius, double vRadius);

public:
  MORFEUS_LIB_DECL Point center() const;
  MORFEUS_LIB_DECL int32_t number() const;
  MORFEUS_LIB_DECL std::size_t resolution() const;
  MORFEUS_LIB_DECL double uRadius() const;
  MORFEUS_LIB_DECL double vRadius() const;
  MORFEUS_LIB_DECL void setCenter(const Point & pt);
  MORFEUS_LIB_DECL void setResolution(std::size_t value);
  MORFEUS_LIB_DECL void setURadius(double value);
  MORFEUS_LIB_DECL void setVRadius(double value);

public:
  MORFEUS_LIB_DECL static int32_t totalCreated();

protected:
  bool doContainsPoint(const Point & pt) const override;
  std::vector<Face*> doGetFaceList() const override;
  std::vector<Region*> doGetRegionList() const override;
  std::vector<Segment*> doGetSegmentList() const override;
  std::vector<Vertex*> doGetVertexList() const override;
  std::vector<Wire*> doGetWireList() const override;
  void doPrint(std::ostream & output, int tabPos = 0) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  void init(const Point & pt, double uRadius, double vRadius);
  void updatePosition();

private:
  typedef std::unique_ptr<Face> FacePtr;
  typedef std::unique_ptr<Segment> SegmentPtr;
  typedef std::unique_ptr<Vertex> VertexPtr;
  typedef std::vector<SegmentPtr> SegmentList;
  typedef std::vector<VertexPtr> VertexList;

private:
  static int32_t mTotal;
  int32_t mNumber;
  std::size_t mResolution;
  double mRadiusU;
  double mRadiusV;
  Point mCenter;
  FacePtr mFace;
  SegmentList mSegmentList;
  VertexList mVertexList;
};

inline Point Ellipse::center() const
{
  return mCenter;
}

inline int32_t Ellipse::number() const
{
  return mNumber;
}

inline std::size_t Ellipse::resolution() const
{
  return mResolution;
}

inline double Ellipse::uRadius() const
{
  return mRadiusU;
}

inline double Ellipse::vRadius() const
{
  return mRadiusV;
}

inline int32_t Ellipse::totalCreated()
{
  return mTotal;
}

}
}
}

#endif // MORFEUS_MODEL_GEOMETRY_ELLIPSE_H
