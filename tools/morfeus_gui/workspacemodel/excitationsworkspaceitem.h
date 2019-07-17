#ifndef EXCITATIONSWORKSPACEITEM_H
#define EXCITATIONSWORKSPACEITEM_H

#include "workspacemodelitem.h"

class ExcitationsWorkspaceItem
    : public WorkspaceModelItem
{
public:
  ExcitationsWorkspaceItem();

protected:
  QList<Action*> doGetActionList() const override;
};

#endif // EXCITATIONSWORKSPACEITEM_H
