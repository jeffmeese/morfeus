#ifndef REGION_H
#define REGION_H

#include "morfeus.h"
#include "types.h"

class Region
{
public:
  MORFEUS_LIB_DECL Region(int32_t id);
  MORFEUS_LIB_DECL Region(int32_t id, double x, double y);

public:
  MORFEUS_LIB_DECL double attribute() const;
  MORFEUS_LIB_DECL int32_t id() const;
  MORFEUS_LIB_DECL double maxArea() const;
  MORFEUS_LIB_DECL double x() const;
  MORFEUS_LIB_DECL double y() const;
  MORFEUS_LIB_DECL void setAttribute(double value);
  MORFEUS_LIB_DECL void setMaxArea(double value);
  MORFEUS_LIB_DECL void setPosition(double x, double y);
  MORFEUS_LIB_DECL void setX(double value);
  MORFEUS_LIB_DECL void setY(double value);

private:
  int32_t mId;
  double mAttribute;
  double mMaxArea;
  double mX;
  double mY;
};

#endif // REGION_H
