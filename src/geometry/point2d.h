#ifndef POINT2D_H
#define POINT2D_H

#include "morfeus.h"

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

private:
  double mX;
  double mY;
};

#endif // POINT2D_H
