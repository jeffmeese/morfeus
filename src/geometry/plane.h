#ifndef PLANE_H
#define PLANE_H

#include "shape.h"
#include "point3d.h"

namespace Morfeus {
namespace Geometry {

class Plane
    : public Shape
{
public:
  MORFEUS_LIB_DECL Plane();
  MORFEUS_LIB_DECL Plane(const Point3D & origin, const Point3D & point1, const Point3D & point2);
  MORFEUS_LIB_DECL Plane(const std::string & name);
  MORFEUS_LIB_DECL Plane(const std::string & name, const Point3D & origin, const Point3D & point1, const Point3D & point2);

public:
  MORFEUS_LIB_DECL Point3D origin() const;
  MORFEUS_LIB_DECL Point3D point1() const;
  MORFEUS_LIB_DECL Point3D point2() const;
  MORFEUS_LIB_DECL void setOrigin(const Point3D & pt);
  MORFEUS_LIB_DECL void point1(const Point3D & pt);
  MORFEUS_LIB_DECL void point2(const Point3D & pt);

public:
  MORFEUS_LIB_DECL Point3D computePoint3() const;

protected:
  std::vector<Face> doGetFacetList() const override;
  std::vector<Segment> doGetSegmentList() const override;
  std::vector<Vertex> doGetVertexList() const override;
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  Point3D mOrigin;
  Point3D mPoint1;
  Point3D mPoint2;
  Point3D mPoint3;
};

inline Point3D Plane::origin() const
{
  return mOrigin;
}

inline Point3D Plane::point1() const
{
  return mPoint1;
}

inline Point3D Plane::point2() const
{
  return mPoint2;
}

inline void Plane::setOrigin(const Point3D & pt)
{
  mOrigin = pt;
  mPoint3 = computePoint3();
}

inline void Plane::point1(const Point3D & pt)
{
  mPoint1 = pt;
  mPoint3 = computePoint3();
}

inline void Plane::point2(const Point3D & pt)
{
  mPoint2 = pt;
  mPoint3 = computePoint3();
}

}
}

#endif // PLANE_H
