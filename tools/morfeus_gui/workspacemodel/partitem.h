#ifndef PARTITEM_H
#define PARTITEM_H

#include "workspacemodelitem.h"

#include <model/geometry/part.h>

class PartItem
    : public WorkspaceModelItem
{
public:
  PartItem(morfeus::model::geometry::Part * part);

public:
  morfeus::model::geometry::Part * part();

public:
  QVariant data(int role = Qt::DisplayRole) const override;
  void setData(const QVariant & data, int role = Qt::UserRole+1) override;

protected:
  QList<Action*> doGetActionList() const override;

private:
  morfeus::model::geometry::Part * mPart;
};

inline morfeus::model::geometry::Part * PartItem::part()
{
  return mPart;
}

#endif // PARTITEM_H
