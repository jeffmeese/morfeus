#ifndef MORFEUS_GEOMETRY_BOX_H
#define MORFEUS_GEOMETRY_BOX_H

#include "shape.h"
#include "point3d.h"

namespace morfeus {
namespace geometry {

class Box
    : public Shape
{
public:
  MORFEUS_LIB_DECL Box();
  MORFEUS_LIB_DECL Box(const Point3D & center, double length, double width, double height);
  MORFEUS_LIB_DECL Box(const std::string & name);
  MORFEUS_LIB_DECL Box(const std::string & name, const Point3D & center, double length, double width, double height);

public:
  MORFEUS_LIB_DECL Point3D center() const;
  MORFEUS_LIB_DECL double height() const;
  MORFEUS_LIB_DECL double length() const;
  MORFEUS_LIB_DECL double width() const;
  MORFEUS_LIB_DECL void setCenter(const Point3D & pt);
  MORFEUS_LIB_DECL void setHeight(double value);
  MORFEUS_LIB_DECL void setLength(double value);
  MORFEUS_LIB_DECL void setWidth(double value);

protected:
  std::vector<Face> doGetFacetList() const override;
  std::vector<Segment> doGetSegmentList() const override;
  std::vector<Vertex> doGetVertexList() const override;
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  Point3D mCenter;
  double mHeight;
  double mLength;
  double mWidth;
};

inline Point3D Box::center() const
{
  return mCenter;
}

inline double Box::height() const
{
  return mHeight;
}

inline double Box::length() const
{
  return mLength;
}

inline double Box::width() const
{
  return mWidth;
}

inline void Box::setCenter(const Point3D & pt)
{
  mCenter = pt;
}

inline void Box::setHeight(double value)
{
  mHeight = value;
}

inline void Box::setLength(double value)
{
  mLength = value;
}

inline void Box::setWidth(double value)
{
  mWidth = value;
}

}
}

#endif // MORFEUS_GEOMETRY_BOX_H
