#ifndef RECTANGLEWORKSPACEITEM_H
#define RECTANGLEWORKSPACEITEM_H

#include "workspacemodelitem.h"

class RectangleItem;

class RectangleWorkspaceItem
    : public WorkspaceModelItem
{
public:
  RectangleWorkspaceItem(RectangleItem * item);

public:
  QVariant data(int role = Qt::DisplayRole) const override;
  void setData(const QVariant & data, int role = Qt::UserRole+1) override;

private:
  RectangleItem * mItem;
};

#endif // RECTANGLEWORKSPACEITEM_H
