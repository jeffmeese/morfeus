#include "excitationsworkspaceitem.h"

#include "workspaceview.h"

#include <action.h>
#include <actionmanager.h>

#include <QUuid>

ExcitationsWorkspaceItem::ExcitationsWorkspaceItem()
  : WorkspaceModelItem (QUuid::createUuid().toString())
{
  setText("Excitations");
  setEditable(false);
}

QList<Action*> ExcitationsWorkspaceItem::doGetActionList() const
{
  ActionManager & actionManager = WorkspaceView::actionManager();

  QList<Action*> actionList;
  return actionList;
}
