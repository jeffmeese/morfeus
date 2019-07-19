#ifndef TRIANGLESHAPE_H
#define TRIANGLESHAPE_H

#include "polygon.h"

class TriangleShape
    : public Polygon
{
public:
  MORFEUS_LIB_DECL TriangleShape();

protected:
  void doAddToGeometry(Geometry * geometry) const override;
  void doReadFromXml(ptree & tree) override;
  void doWriteToXml(ptree & tree) const override;
};

#endif // TRIANGLESHAPE_H
