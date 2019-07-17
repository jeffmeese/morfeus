#include "observationsworkspaceitem.h"

#include "workspaceview.h"

#include <action.h>
#include <actionmanager.h>

#include <QUuid>

ObservationsWorkspaceItem::ObservationsWorkspaceItem()
  : WorkspaceModelItem (QUuid::createUuid().toString())
{
  setText("Observations");
  setEditable(false);
}

QList<Action*> ObservationsWorkspaceItem::doGetActionList() const
{
  ActionManager & actionManager = WorkspaceView::actionManager();

  QList<Action*> actionList;
  return actionList;
}
