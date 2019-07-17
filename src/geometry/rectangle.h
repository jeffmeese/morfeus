#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "polygon.h"
#include "point2d.h"

class Rectangle
    : public Polygon
{
public:
  MORFEUS_LIB_DECL Rectangle();

public:
  MORFEUS_LIB_DECL Point2D center() const;
  MORFEUS_LIB_DECL double height() const;
  MORFEUS_LIB_DECL double width() const;

private:
  Point2D mCenter;
  double mHeight;
  double mWidth;
};

inline Point2D Rectangle::center() const
{
  return mCenter;
}

inline double Rectangle::height() const
{
  return mHeight;
}

inline double Rectangle::width() const
{
  return mWidth;
}

#endif // RECTANGLE_H
