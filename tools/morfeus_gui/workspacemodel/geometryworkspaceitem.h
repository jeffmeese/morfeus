#ifndef GEOMETRYWORKSPACEITEM_H
#define GEOMETRYWORKSPACEITEM_H

#include "workspacemodelitem.h"

class GeometryWorkspaceItem
    : public WorkspaceModelItem
{
  Q_OBJECT

public:
  GeometryWorkspaceItem();

protected:
  QList<Action*> doGetActionList() const override;
};

#endif // GEOMETRYWORKSPACEITEM_H
