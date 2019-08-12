#ifndef CUBEITEM_H
#define CUBEITEM_H

#include "partitem.h"

#include <model/geometry/cuboid.h>

class CubeItem
    : public PartItem
{
public:
  CubeItem(morfeus::model::geometry::Cuboid * cuboid);

protected:
  QList<Action*> doGetActionList() const override;

private:
  morfeus::model::geometry::Cuboid * mCuboid;
};

#endif // CUBEITEM_H
