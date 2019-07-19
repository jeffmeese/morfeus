#ifndef HOLE_H
#define HOLE_H

#include "morfeus.h"

#include "shape.h"

class Hole
    : public Shape
{
public:
  MORFEUS_LIB_DECL Hole();
  MORFEUS_LIB_DECL Hole(double x, double y);

public:
  MORFEUS_LIB_DECL double x() const;
  MORFEUS_LIB_DECL double y() const;
  MORFEUS_LIB_DECL void setPosition(double x, double y);
  MORFEUS_LIB_DECL void setX(double value);
  MORFEUS_LIB_DECL void setY(double value);

protected:
  void doReadFromXml(ptree & tree) override;
  void doWriteToXml(ptree & tree) const override;
  void doAddToGeometry(Geometry * geometry) const override;

private:
  double mX;
  double mY;
};

#endif // HOLE_H
