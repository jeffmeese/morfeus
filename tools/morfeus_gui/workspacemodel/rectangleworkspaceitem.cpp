#include "rectangleworkspaceitem.h"
#include "rectangleitem.h"

RectangleWorkspaceItem::RectangleWorkspaceItem(RectangleItem * item)
  : mItem(item)
{
}

QVariant RectangleWorkspaceItem::data(int role) const
{
  if (role == Qt::DisplayRole) {
    return mItem->name();
  }

  return WorkspaceModelItem::data(role);
}

void RectangleWorkspaceItem::setData(const QVariant & value, int role)
{
  if (role == Qt::DisplayRole || Qt::EditRole) {
    mItem->setName(value.toString());
  }

  WorkspaceModelItem::setData(value, role);
}
