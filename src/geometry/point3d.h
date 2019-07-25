#ifndef POINT3D_H
#define POINT3D_H

#include "morfeus.h"

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
  MORFEUS_LIB_DECL friend std::ostream & operator<<(std::ostream & output, const Point3D & point);

private:
  double mX;
  double mY;
  double mZ;
};

#endif // POINT3D_H
