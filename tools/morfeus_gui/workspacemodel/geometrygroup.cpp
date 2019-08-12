#include "geometrygroup.h"

#include <QUuid>

GeometryGroup::GeometryGroup()
  : WorkspaceModelItem (QUuid::createUuid().toString())
{
  setText("Geometry");
  setEditable(false);
}

QList<Action*> GeometryGroup::doGetActionList() const
{
  QList<Action*> actionList;
  return actionList;
}
