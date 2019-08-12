#include "workspacemodel.h"

#include "excitationsworkspaceitem.h"
#include "geometrygroup.h"
#include "modelgroup.h"
#include "observationsworkspaceitem.h"
#include "partitem.h"

#include <model/model.h>
#include <model/geometry/part.h>
#include <model/ports/port.h>

#include <actionmanager.h>
#include <project.h>

#include <sstream>

#include <QDebug>

WorkspaceModel::WorkspaceModel()
  : mExcitationsItems(nullptr)
  , mModelGroup(nullptr)
  , mObservationsItem(nullptr)
  , mProject(new morfeus::core::MorfeusProject)
{
  createDefaultItems();
}

void WorkspaceModel::addPart(std::unique_ptr<morfeus::model::geometry::Part> part)
{
  morfeus::model::geometry::Part * pPart(part.get());
  PartItem * partItem = new PartItem(part.get());
  mModelGroup->geometryGroup()->appendRow(partItem);
  mProject->model()->addPart(std::move(part));
  mProject->setModified(true);
  emit partAdded(pPart);
}

void WorkspaceModel::addPort(std::unique_ptr<morfeus::model::ports::Port> part)
{

}

void WorkspaceModel::createDefaultItems()
{
  mModelGroup = new ModelGroup;
  mExcitationsItems = new ExcitationsWorkspaceItem;
  mObservationsItem = new ObservationsWorkspaceItem;

  invisibleRootItem()->appendRow(mModelGroup);
  invisibleRootItem()->appendRow(mExcitationsItems);
  invisibleRootItem()->appendRow(mObservationsItem);
}

void WorkspaceModel::loadProject()
{
}
