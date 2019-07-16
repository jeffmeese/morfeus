#include "workspacemodel.h"

#include <project.h>

#include "rectangleitem.h"
#include "rectangleworkspaceitem.h"

WorkspaceModel::WorkspaceModel()
  : mGeometryItem(nullptr)
  , mMeshItem(nullptr)
  , mExcitationsItems(nullptr)
  , mObservationsItem(nullptr)
  , mCurrentParent(nullptr)
{
  createDefaultItems();
}

void WorkspaceModel::createDefaultItems()
{
  mGeometryItem = new QStandardItem("Geometry");
  mMeshItem = new QStandardItem("Mesh");
  mExcitationsItems = new QStandardItem("Excitations");
  mObservationsItem = new QStandardItem("Observations");

  invisibleRootItem()->appendRow(mGeometryItem);
  invisibleRootItem()->appendRow(mMeshItem);
  invisibleRootItem()->appendRow(mExcitationsItems);
  invisibleRootItem()->appendRow(mObservationsItem);
}

QStandardItem * WorkspaceModel::geometryItem()
{
  return mGeometryItem;
}

void WorkspaceModel::handleAddItem(ProjectItem *projectItem)
{
  RectangleItem * rectItem = dynamic_cast<RectangleItem*>(projectItem);
  RectangleWorkspaceItem * item = new RectangleWorkspaceItem(rectItem);
  QStandardItem * parentItem = (mCurrentParent != nullptr) ? mCurrentParent : invisibleRootItem();
  parentItem->appendRow(item);
}

void WorkspaceModel::loadProject()
{

}

void WorkspaceModel::setCurrentParent(QStandardItem *item)
{
  mCurrentParent = item;
}

void WorkspaceModel::setProject(Project *project)
{
  removeRows(0, rowCount());
  createDefaultItems();

  mProject = project;
  if (mProject != nullptr) {
    loadProject();
    connect(mProject, SIGNAL(itemAdded(ProjectItem*)), SLOT(handleAddItem(ProjectItem*)));
  }
}
