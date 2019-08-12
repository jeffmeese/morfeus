#include "cubeitem.h"

#include <QUuid>

CubeItem::CubeItem(morfeus::model::geometry::Cuboid * cuboid)
  : PartItem (cuboid)
  , mCuboid(cuboid)
{
}

QList<Action*> CubeItem::doGetActionList() const
{
  QList<Action*> actionList;
  return actionList;
}

