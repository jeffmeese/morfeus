#ifndef MODELGROUP_H
#define MODELGROUP_H

#include "workspacemodelitem.h"

#include <model/geometry/cuboid.h>

class GeometryGroup;

class ModelGroup
    : public WorkspaceModelItem
{
public:
  ModelGroup();

public:
  GeometryGroup * geometryGroup();

public:
  void addCuboid(morfeus::model::geometry::Cuboid * cuboid);

protected:
  QList<Action*> doGetActionList() const override;

private:
  GeometryGroup * mGeometryGroup;
};

inline GeometryGroup * ModelGroup::geometryGroup()
{
  return mGeometryGroup;
}

#endif // MODELGROUP_H
