#ifndef WORKSPACEMODEL_H
#define WORKSPACEMODEL_H

#include <QStandardItemModel>

#include <model/geometry/part.h>
#include <model/media/medium.h>
#include <model/ports/port.h>

#include <core/morfeusproject.h>

class ActionManager;
class ExcitationsWorkspaceItem;
class GeometryWorkspaceItem;
class ModelGroup;
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
  void addPart(std::unique_ptr<morfeus::model::geometry::Part> part);
  void addPort(std::unique_ptr<morfeus::model::ports::Port> part);

private:
  void createDefaultItems();
  void loadProject();

signals:
  void partAdded(morfeus::model::geometry::Part * part);
  void portAdded(morfeus::model::ports::Port * port);

private:
  std::unique_ptr<morfeus::core::MorfeusProject> mProject;
  ExcitationsWorkspaceItem * mExcitationsItems;
  ModelGroup * mModelGroup;
  ObservationsWorkspaceItem * mObservationsItem;
};

#endif // WORKSPACEMODEL_H
