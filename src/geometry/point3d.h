#ifndef POINT3D_H
#define POINT3D_H

#include "morfeus.h"

#include "rapidxml.hpp"
#include "xmlutils.h"

namespace Morfeus {
namespace Geometry {

class Point3D
{
public:
  MORFEUS_LIB_DECL Point3D();
  MORFEUS_LIB_DECL Point3D(double x, double y, double z);

public:
  MORFEUS_LIB_DECL double x() const;
  MORFEUS_LIB_DECL double y() const;
  MORFEUS_LIB_DECL double z() const;
  MORFEUS_LIB_DECL void setPosition(double x, double y, double z);
  MORFEUS_LIB_DECL void setX(double value);
  MORFEUS_LIB_DECL void setY(double value);
  MORFEUS_LIB_DECL void setZ(double value);

public:
  MORFEUS_LIB_DECL void move(double dx, double dy, double dz);
  MORFEUS_LIB_DECL void print(std::ostream & output, int tabPos = 0) const;
  MORFEUS_LIB_DECL void print(int tabPos = 0) const;
  MORFEUS_LIB_DECL void readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node);
  MORFEUS_LIB_DECL void writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const;
  MORFEUS_LIB_DECL friend std::ostream & operator<<(std::ostream & output, const Point3D & point);

private:
  double mX;
  double mY;
  double mZ;
};

inline void Point3D::setX(double value)
{
  mX = value;
}

inline void Point3D::setY(double value)
{
  mY = value;
}

inline void Point3D::setZ(double value)
{
  mZ = value;
}

inline double Point3D::x() const
{
  return mX;
}

inline double Point3D::y() const
{
  return mY;
}

inline double Point3D::z() const
{
  return mZ;
}

}
}

#endif // POINT3D_H
