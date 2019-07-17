#include "meshworkspaceitem.h"

#include "workspaceview.h"

#include <action.h>
#include <actionmanager.h>

#include <QUuid>

MeshWorkspaceItem::MeshWorkspaceItem()
  : WorkspaceModelItem (QUuid::createUuid().toString())
{
  setText("Mesh");
  setEditable(false);
}

QList<Action*> MeshWorkspaceItem::doGetActionList() const
{
  ActionManager & actionManager = WorkspaceView::actionManager();

  QList<Action*> actionList;
  return actionList;
}
