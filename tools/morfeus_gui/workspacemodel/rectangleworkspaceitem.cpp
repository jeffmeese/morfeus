#include "rectangleworkspaceitem.h"

#include "actionmanager.h"
#include "rectangleitem.h"
#include "rectangle.h"
#include "renamecommand.h"
#include "workspaceview.h"

#include "action.h"

static const QString OBJECT_ID("Rectangle");

RectangleWorkspaceItem::RectangleWorkspaceItem(RectangleItem * item)
  : WorkspaceModelItem(item->id())
  , mItem(item)
{
}

QVariant RectangleWorkspaceItem::data(int role) const
{
  if (role == Qt::DisplayRole) {
    return mItem->name();
  }

  return WorkspaceModelItem::data(role);
}

QList<Action*> RectangleWorkspaceItem::doGetActionList() const
{
  ActionManager & actionManager = WorkspaceView::actionManager();
  QList<Action*> actionList;
  actionList.append(actionManager.action(RenameCommand::commandId()));
  return actionList;
}

void RectangleWorkspaceItem::setData(const QVariant & value, int role)
{
  if (role == Qt::DisplayRole || Qt::EditRole) {
    mItem->setName(value.toString());
  }

  WorkspaceModelItem::setData(value, role);
}

namespace  {

  WorkspaceModelItem * createFunction(ProjectItem * item)
  {
    RectangleItem * rectangle = dynamic_cast<RectangleItem*>(item);
    return new RectangleWorkspaceItem(rectangle);
  }

  const bool registered = WorkspaceModelItem::Factory::Instance().Register(OBJECT_ID, createFunction);
}
