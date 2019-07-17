#ifndef WORKSPACEMODEL_H
#define WORKSPACEMODEL_H

#include <QStandardItemModel>

class ActionManager;
class ExcitationsWorkspaceItem;
class GeometryWorkspaceItem;
class MeshWorkspaceItem;
class ObservationsWorkspaceItem;
class GuiProject;
class ProjectItem;
class RectangleItem;

class WorkspaceModel
    : public QStandardItemModel
{
  Q_OBJECT

public:
  WorkspaceModel();

public:
  GeometryWorkspaceItem * geometryItem();
  void setProject(GuiProject * project);

private slots:
  void handleAddRectangle(ProjectItem * rectItem);
  void handleAboutToRemoveItem(ProjectItem * projectItem);

private:
  void createDefaultItems();
  void loadProject();

private:
  GuiProject * mProject;
  ExcitationsWorkspaceItem * mExcitationsItems;
  GeometryWorkspaceItem * mGeometryItem;
  MeshWorkspaceItem * mMeshItem;
  ObservationsWorkspaceItem * mObservationsItem;
};

#endif // WORKSPACEMODEL_H
