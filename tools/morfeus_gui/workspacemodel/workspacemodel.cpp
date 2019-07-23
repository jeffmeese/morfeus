#include "workspacemodel.h"

#include "excitationsworkspaceitem.h"
#include "geometryworkspaceitem.h"
#include "meshworkspaceitem.h"
#include "guiproject.h"
#include "observationsworkspaceitem.h"
#include "rectangleitem.h"
#include "rectangleworkspaceitem.h"

#include <actionmanager.h>
#include <project.h>

#include <sstream>

#include <QDebug>

WorkspaceModel::WorkspaceModel()
  : mExcitationsItems(nullptr)
  , mGeometryItem(nullptr)
  , mMeshItem(nullptr)
  , mObservationsItem(nullptr)
{
  createDefaultItems();
}

void WorkspaceModel::createDefaultItems()
{
  mGeometryItem = new GeometryWorkspaceItem;
  mMeshItem = new MeshWorkspaceItem;
  mExcitationsItems = new ExcitationsWorkspaceItem;
  mObservationsItem = new ObservationsWorkspaceItem;

  invisibleRootItem()->appendRow(mGeometryItem);
  invisibleRootItem()->appendRow(mMeshItem);
  invisibleRootItem()->appendRow(mExcitationsItems);
  invisibleRootItem()->appendRow(mObservationsItem);
}

GeometryWorkspaceItem * WorkspaceModel::geometryItem()
{
  return mGeometryItem;
}

void WorkspaceModel::handleAboutToRemoveItem(ProjectItem *projectItem)
{
  qDebug() << QObject::tr(__FUNCTION__);

  for (int i = 0; i < mGeometryItem->rowCount(); i++) {
    QStandardItem * item = mGeometryItem->child(i);
    WorkspaceModelItem * childItem = dynamic_cast<WorkspaceModelItem*>(item);
    if (childItem != nullptr && childItem->id() == projectItem->id()) {
      mGeometryItem->removeRow(i);
      emit layoutChanged();
    }
  }
}

void WorkspaceModel::handleAddRectangle(ProjectItem *projectItem)
{
  qDebug() << QObject::tr(__FUNCTION__);

  //typedef WorkspaceModelItem::Factory Factory;

//  try {
//    WorkspaceModelItem * item = Factory::Instance().CreateObject(projectItem->typeId(), projectItem);
//    mGeometryItem->appendRow(item);
//  }
//  catch (Loki::DefaultFactoryError<QString, WorkspaceModelItem>::Exception &) {
//    std::ostringstream oss;
//    oss << "Could not create port attribute with type " << projectItem->typeId().toStdString();
//    qDebug() << QString::fromStdString(oss.str());
//  }
}

void WorkspaceModel::loadProject()
{
}

void WorkspaceModel::setProject(GuiProject *project)
{
  removeRows(0, rowCount());
  createDefaultItems();

  mProject = project;
  if (mProject != nullptr) {
    loadProject();
    connect(mProject, SIGNAL(rectangleAdded(ProjectItem*)), SLOT(handleAddRectangle(ProjectItem*)));
    connect(mProject, SIGNAL(itemAboutToBeRemoved(ProjectItem*)), SLOT(handleAboutToRemoveItem(ProjectItem*)));
  }
}
