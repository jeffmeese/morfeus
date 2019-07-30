#ifndef MORFEUS_GEOMETRY_CIRCLE_H
#define MORFEUS_GEOMETRY_CIRCLE_H

#include "shape.h"
#include "point3d.h"

namespace Morfeus {
namespace Geometry {

class Circle
    : public Shape
{
public:
  MORFEUS_LIB_DECL Circle();
  MORFEUS_LIB_DECL Circle(const std::string & name);
  MORFEUS_LIB_DECL Circle(const std::string & id, const std::string & name);
  MORFEUS_LIB_DECL Circle(const Point3D & center, double radius);
  MORFEUS_LIB_DECL Circle(const std::string & name, const Point3D & center, double radius);
  MORFEUS_LIB_DECL Circle(const std::string & id, const std::string & name, const Point3D & center, double radius);

public:
  MORFEUS_LIB_DECL Point3D center() const;
  MORFEUS_LIB_DECL double radius() const;
  MORFEUS_LIB_DECL std::size_t resolution() const;
  MORFEUS_LIB_DECL void setCenter(const Point3D & pt);
  MORFEUS_LIB_DECL void setRadius(double value);
  MORFEUS_LIB_DECL void setResolution(std::size_t value);

protected:
  std::vector<Face> doGetFacetList() const override;
  std::vector<Segment> doGetSegmentList() const override;
  std::vector<Vertex> doGetVertexList() const override;
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  std::size_t mResolution;
  double mRadius;
  Point3D mCenter;
};

inline Point3D Circle::center() const
{
  return mCenter;
}

inline double Circle::radius() const
{
  return mRadius;
}

inline std::size_t Circle::resolution() const
{
  return mResolution;
}

inline void Circle::setCenter(const Point3D &pt)
{
  mCenter = pt;
}

inline void Circle::setRadius(double value)
{
  mRadius =  value;
}

inline void Circle::setResolution(std::size_t value)
{
  mResolution = value;
}

}
}

#endif // MORFEUS_GEOMETRY_CIRCLE_H
