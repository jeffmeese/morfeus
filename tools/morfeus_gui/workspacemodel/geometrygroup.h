#ifndef GEOMETRYGROUP_H
#define GEOMETRYGROUP_H

#include "workspacemodelitem.h"

class GeometryGroup
    : public WorkspaceModelItem
{
public:
  GeometryGroup();

protected:
  QList<Action*> doGetActionList() const override;
};

#endif // GEOMETRYGROUP_H
