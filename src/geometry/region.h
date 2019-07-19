#ifndef REGION_H
#define REGION_H

#include "morfeus.h"
#include "morfeusobject.h"

class Region
    : public MorfeusObject
{
public:
  MORFEUS_LIB_DECL Region();
  MORFEUS_LIB_DECL Region(double x, double y);

public:
  MORFEUS_LIB_DECL double attribute() const;
  MORFEUS_LIB_DECL double maxArea() const;
  MORFEUS_LIB_DECL double x() const;
  MORFEUS_LIB_DECL double y() const;
  MORFEUS_LIB_DECL void setAttribute(double value);
  MORFEUS_LIB_DECL void setMaxArea(double value);
  MORFEUS_LIB_DECL void setPosition(double x, double y);
  MORFEUS_LIB_DECL void setX(double value);
  MORFEUS_LIB_DECL void setY(double value);

protected:
  void doReadFromXml(ptree & tree) override;
  void doWriteToXml(ptree & tree) const override;

private:
  double mAttribute;
  double mMaxArea;
  double mX;
  double mY;
};

#endif // REGION_H
