#ifndef OBSERVATIONSWORKSPACEITEM_H
#define OBSERVATIONSWORKSPACEITEM_H

#include "workspacemodelitem.h"

class ObservationsWorkspaceItem
    : public WorkspaceModelItem
{
public:
  ObservationsWorkspaceItem();

protected:
  QList<Action*> doGetActionList() const override;
};

#endif // OBSERVATIONSWORKSPACEITEM_H
