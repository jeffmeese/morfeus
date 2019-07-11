#ifndef HOLE_H
#define HOLE_H

#include "morfeus.h"

class Hole
{
public:
  MORFEUS_LIB_DECL Hole(int32_t id);
  MORFEUS_LIB_DECL Hole(int32_t id, double x, double y);

public:
  MORFEUS_LIB_DECL int32_t id() const;
  MORFEUS_LIB_DECL double x() const;
  MORFEUS_LIB_DECL double y() const;
  MORFEUS_LIB_DECL void setPosition(double x, double y);
  MORFEUS_LIB_DECL void setX(double value);
  MORFEUS_LIB_DECL void setY(double value);

private:
  int32_t mId;
  double mX;
  double mY;
};

#endif // HOLE_H
