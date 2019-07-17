#include "geometryworkspaceitem.h"

#include "addrectanglecommand.h"
#include "workspaceview.h"

#include <action.h>
#include <actionmanager.h>

#include <QUuid>

GeometryWorkspaceItem::GeometryWorkspaceItem()
  : WorkspaceModelItem (QUuid::createUuid().toString())
{
  setText("Geometry");
  setEditable(false);
}

QList<Action*> GeometryWorkspaceItem::doGetActionList() const
{
  ActionManager & actionManager = WorkspaceView::actionManager();

  QList<Action*> actionList;
  actionList.push_back(actionManager.action(AddRectangleCommand::commandId()));
  return actionList;
}

