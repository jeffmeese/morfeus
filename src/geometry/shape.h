#ifndef SHAPE_H
#define SHAPE_H

#include "morfeus.h"
#include "morfeusobject.h"

#include <string>

class Geometry;

class Shape
    : public MorfeusObject
{
public:
  MORFEUS_LIB_DECL void addToGeometry(Geometry * geometry) const;

protected:
  Shape();

protected:
  virtual void doAddToGeometry(Geometry * geometry) const = 0;

private:
  std::string mName;
};

#endif // SHAPE_H
