#include "partitem.h"

PartItem::PartItem(morfeus::model::geometry::Part * part)
  : WorkspaceModelItem(QString::fromStdString(part->id()))
  , mPart(part)
{
}

QList<Action*> PartItem::doGetActionList() const
{
  QList<Action*> actionList;
  return actionList;
}

QVariant PartItem::data(int role) const
{
  if (role == Qt::DisplayRole) {
    return QString::fromStdString(mPart->name());
  }

  return WorkspaceModelItem::data(role);
}

void PartItem::setData(const QVariant & value, int role)
{
  if (role == Qt::DisplayRole || Qt::EditRole) {
    mPart->setName(value.toString().toStdString());
  }

  WorkspaceModelItem::setData(value, role);
}
