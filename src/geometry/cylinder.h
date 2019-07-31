#ifndef MORFEUS_GEOMETRY_CYLINDER_H
#define MORFEUS_GEOMETRY_CYLINDER_H

#include "solid.h"
#include "point.h"

namespace morfeus {
namespace geometry {

class Cylinder
    : public Solid
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
  MORFEUS_LIB_DECL double radius() const;
  MORFEUS_LIB_DECL std::size_t resolution() const;
  MORFEUS_LIB_DECL void setCenter(const Point & pt);
  MORFEUS_LIB_DECL void setHeight(double value);
  MORFEUS_LIB_DECL void setRadius(double value);
  MORFEUS_LIB_DECL void setResolution(std::size_t value);

protected:
  std::vector<Face*> doGetFaceList() const override;
  std::vector<Region*> doGetRegionList() const override;
  std::vector<Segment*> doGetSegmentList() const override;
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  std::size_t mResolution;
  double mHeight;
  double mRadius;
  Point mCenter;
};

inline Point Cylinder::center() const
{
  return mCenter;
}


inline double Cylinder::height() const
{
  return mHeight;
}

inline double Cylinder::radius() const
{
  return mRadius;
}

inline std::size_t Cylinder::resolution() const
{
  return mResolution;
}

inline void Cylinder::setCenter(const Point & pt)
{
  mCenter = pt;
}

inline void Cylinder::setHeight(double value)
{
  mHeight = value;
}

inline void Cylinder::setRadius(double value)
{
  mRadius = value;
}

inline void Cylinder::setResolution(std::size_t value)
{
  mResolution = value;
}

}
}

#endif // MORFEUS_GEOMETRY_CYLINDER_H
