#ifndef MORFEUS_GEOMETRY_POINT_H
#define MORFEUS_GEOMETRY_POINT_H

#include "morfeus.h"

#include "core/morfeusobject.h"

namespace morfeus {
namespace geometry {

class Point
    : public core::MorfeusObject
{
public:
  MORFEUS_LIB_DECL Point();
  MORFEUS_LIB_DECL Point(double x, double y, double z);

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

protected:
  void doPrint(std::ostream & output, int tabPos = 0) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  double mX;
  double mY;
  double mZ;
};

inline void Point::setX(double value)
{
  mX = value;
}

inline void Point::setY(double value)
{
  mY = value;
}

inline void Point::setZ(double value)
{
  mZ = value;
}

inline double Point::x() const
{
  return mX;
}

inline double Point::y() const
{
  return mY;
}

inline double Point::z() const
{
  return mZ;
}

}
}

#endif // MORFEUS_GEOMETRY_POINT_H
