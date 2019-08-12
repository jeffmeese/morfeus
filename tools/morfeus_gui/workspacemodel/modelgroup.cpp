#include "modelgroup.h"
#include "geometrygroup.h"
#include "cubeitem.h"

#include <QUuid>

ModelGroup::ModelGroup()
  : WorkspaceModelItem (QUuid::createUuid().toString())
{
  setText("Model");
  setEditable(false);

  mGeometryGroup = new GeometryGroup;
  appendRow(mGeometryGroup);
}

void ModelGroup::addCuboid(morfeus::model::geometry::Cuboid * cuboid)
{
  CubeItem * cubeItem = new CubeItem(cuboid);
  cubeItem->setText(QString::fromStdString(cuboid->name()));
  mGeometryGroup->appendRow(cubeItem);
}

QList<Action*> ModelGroup::doGetActionList() const
{
  QList<Action*> actionList;
  return actionList;
}
