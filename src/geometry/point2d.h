#ifndef POINT2D_H
#define POINT2D_H

#include "morfeus.h"

#include "rapidxml.hpp"
#include "xmlutils.h"

namespace Morfeus {
namespace Geometry {

class Point2D
{
public:
  MORFEUS_LIB_DECL Point2D();
  MORFEUS_LIB_DECL Point2D(double x, double y);

public:
  MORFEUS_LIB_DECL double x() const;
  MORFEUS_LIB_DECL double y() const;
  MORFEUS_LIB_DECL void setPosition(double x, double y);
  MORFEUS_LIB_DECL void setX(double value);
  MORFEUS_LIB_DECL void setY(double value);

public:
  MORFEUS_LIB_DECL void move(double dx, double dy);
  MORFEUS_LIB_DECL void print(std::ostream & output, int tabPos = 0) const;
  MORFEUS_LIB_DECL void print(int tabPos = 0) const;
  MORFEUS_LIB_DECL void readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node);
  MORFEUS_LIB_DECL void writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const;
  MORFEUS_LIB_DECL friend std::ostream & operator<<(std::ostream & output, const Point2D & point);

private:
  double mX;
  double mY;
};

inline void Point2D::setX(double value)
{
  mX = value;
}

inline void Point2D::setY(double value)
{
  mY = value;
}

inline double Point2D::x() const
{
  return mX;
}

inline double Point2D::y() const
{
  return mY;
}

}
}

#endif // POINT2D_H
