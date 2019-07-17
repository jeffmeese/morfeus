#ifndef MESHWORKSPACEITEM_H
#define MESHWORKSPACEITEM_H

#include "workspacemodelitem.h"

class MeshWorkspaceItem
    : public WorkspaceModelItem
{
public:
  MeshWorkspaceItem();

protected:
  QList<Action*> doGetActionList() const override;
};

#endif // MESHWORKSPACEITEM_H
