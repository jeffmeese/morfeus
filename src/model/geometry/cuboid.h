#ifndef MORFEUS_MODEL_GEOMETRY_CUBOID_H
#define MORFEUS_MODEL_GEOMETRY_CUBOID_H

#include "part.h"
#include "point.h"

namespace morfeus {
namespace model {
namespace geometry {

class Cuboid
    : public Part
{
public:
  MORFEUS_LIB_DECL Cuboid();
  MORFEUS_LIB_DECL Cuboid(const std::string & name);
  MORFEUS_LIB_DECL Cuboid(const std::string & id, const std::string & name);
  MORFEUS_LIB_DECL Cuboid(const Point & pt, double length, double width, double height);
  MORFEUS_LIB_DECL Cuboid(const std::string & name, const Point & pt, double length, double width, double height);
  MORFEUS_LIB_DECL Cuboid(const std::string & id, const std::string & name, const Point & pt, double length, double width, double height);

public:
  MORFEUS_LIB_DECL Point baseCenter() const;
  MORFEUS_LIB_DECL double height() const;
  MORFEUS_LIB_DECL double length() const;
  MORFEUS_LIB_DECL int32_t number() const;
  MORFEUS_LIB_DECL double width() const;
  MORFEUS_LIB_DECL void setBaseCenter(const Point & pt);
  MORFEUS_LIB_DECL void setBaseCenter(double x, double y, double z);
  MORFEUS_LIB_DECL void setHeight(double value);
  MORFEUS_LIB_DECL void setLength(double value);
  MORFEUS_LIB_DECL void setWidth(double value);

public:
  MORFEUS_LIB_DECL static void resetTotal();
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
  void init(const Point & pt, double length, double width, double height);
  void updatePosition();

private:
  typedef std::unique_ptr<Face> FacePtr;
  typedef std::unique_ptr<Region> RegionPtr;
  typedef std::unique_ptr<Segment> SegmentPtr;
  typedef std::unique_ptr<Vertex> VertexPtr;
  typedef std::vector<FacePtr> FaceVector;
  typedef std::vector<SegmentPtr> SegmentVector;
  typedef std::vector<VertexPtr> VertexVector;

private:
  static int mTotal;
  int32_t mNumber;
  double mHeight;
  double mLength;
  double mWidth;
  Point mBaseCenter;
  RegionPtr mRegion;
  FaceVector mFaces;
  SegmentVector mSegments;
  VertexVector mVertices;
};

inline Point Cuboid::baseCenter() const
{
  return mBaseCenter;
}

inline double Cuboid::height() const
{
  return mHeight;
}

inline double Cuboid::length() const
{
  return mLength;
}

inline int32_t Cuboid::number() const
{
  return mNumber;
}

inline double Cuboid::width() const
{
  return mWidth;
}

inline int32_t Cuboid::totalCreated()
{
  return mTotal;
}

}
}
}

#endif // MORFEUS_MODEL_GEOMETRY_CUBOID_H
