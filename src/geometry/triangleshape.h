#ifndef TRIANGLESHAPE_H
#define TRIANGLESHAPE_H

#include "shape.h"
#include "point3d.h"

namespace morfeus {
namespace geometry {

class TriangleShape
    : public Shape
{
public:
  MORFEUS_LIB_DECL TriangleShape();
  MORFEUS_LIB_DECL TriangleShape(const std::string & name);
  MORFEUS_LIB_DECL TriangleShape(const std::string & id, const std::string & name);
  MORFEUS_LIB_DECL TriangleShape(const Point3D & pt1, const Point3D & pt2, const Point3D & pt3);
  MORFEUS_LIB_DECL TriangleShape(const std::string & name, const Point3D & pt1, const Point3D & pt2, const Point3D & pt3);
  MORFEUS_LIB_DECL TriangleShape(const std::string & id, const std::string & name, const Point3D & pt1, const Point3D & pt2, const Point3D & pt3);

public:
  MORFEUS_LIB_DECL Point3D point1() const;
  MORFEUS_LIB_DECL Point3D point2() const;
  MORFEUS_LIB_DECL Point3D point3() const;
  MORFEUS_LIB_DECL void setPosition(const Point3D & pt1, const Point3D & pt2, const Point3D & pt3);
  MORFEUS_LIB_DECL void setPoint1(const Point3D & pt);
  MORFEUS_LIB_DECL void setPoint2(const Point3D & pt);
  MORFEUS_LIB_DECL void setPoint3(const Point3D & pt);

protected:
  std::vector<Face> doGetFacetList() const override;
  std::vector<Segment> doGetSegmentList() const override;
  std::vector<Vertex> doGetVertexList() const override;
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  Point3D mPoint1;
  Point3D mPoint2;
  Point3D mPoint3;
};

inline Point3D TriangleShape::point1() const
{
  return mPoint1;
}

inline Point3D TriangleShape::point2() const
{
  return mPoint1;
}

inline Point3D TriangleShape::point3() const
{
  return mPoint1;
}

inline void TriangleShape::setPoint1(const Point3D &pt)
{
  mPoint1 = pt;
}

inline void TriangleShape::setPoint2(const Point3D &pt)
{
  mPoint2 = pt;
}

inline void TriangleShape::setPoint3(const Point3D &pt)
{
  mPoint3 = pt;
}

}
}

#endif // TRIANGLESHAPE_H
