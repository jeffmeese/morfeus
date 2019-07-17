#include "workspacemodelitem.h"

#include <action.h>

WorkspaceModelItem::WorkspaceModelItem(QString itemId)
  : mItemId(itemId)
{
}

QList<Action*> WorkspaceModelItem::getActionList() const
{
  return doGetActionList();
}

QString WorkspaceModelItem::id() const
{
  return mItemId;
}
