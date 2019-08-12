#ifndef MORFEUS_MODEL_GEOMETRY_RECTANGLE_H
#define MORFEUS_MODEL_GEOMETRY_RECTANGLE_H

#include "part.h"
#include "point.h"

namespace morfeus {
namespace model {
namespace geometry {

class Rectangle
    : public Part
{
public:
  MORFEUS_LIB_DECL Rectangle();
  MORFEUS_LIB_DECL Rectangle(const std::string & name);
  MORFEUS_LIB_DECL Rectangle(const std::string & id, const std::string & name);
  MORFEUS_LIB_DECL Rectangle(const Point & center, double length, double width);
  MORFEUS_LIB_DECL Rectangle(const std::string & name, const Point & center, double length, double width);
  MORFEUS_LIB_DECL Rectangle(const std::string & id, const std::string & name, const Point & center, double length, double width);

public:
  MORFEUS_LIB_DECL Point center() const;
  MORFEUS_LIB_DECL double length() const;
  MORFEUS_LIB_DECL int32_t number() const;
  MORFEUS_LIB_DECL double width() const;
  MORFEUS_LIB_DECL void setCenter(Point center);
  MORFEUS_LIB_DECL void setLength(double value);
  MORFEUS_LIB_DECL void setWidth(double value);

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
  void init(const Point & pt, double length, double width);
  void updatePosition();

private:
  typedef std::unique_ptr<Face> FacePtr;
  typedef std::unique_ptr<Segment> SegmentPtr;
  typedef std::unique_ptr<Vertex> VertexPtr;
  typedef std::vector<VertexPtr> VertexVector;

private:
  static int32_t mTotal;
  int32_t mNumber;
  double mLength;
  double mWidth;
  Point mCenter;
  FacePtr mFace;
  VertexVector mVertices;
};

inline Point Rectangle::center() const
{
  return mCenter;
}

inline double Rectangle::length() const
{
  return mLength;
}

inline int32_t Rectangle::number() const
{
  return mNumber;
}

inline double Rectangle::width() const
{
  return mWidth;
}

inline int32_t Rectangle::totalCreated()
{
  return mTotal;
}

}
}
}

#endif // MORFEUS_MODEL_GEOMETRY_RECTANGLE_H
