#ifndef WORKSPACEMODEL_H
#define WORKSPACEMODEL_H

#include <QStandardItemModel>

class Project;
class ProjectItem;

class WorkspaceModel
    : public QStandardItemModel
{
  Q_OBJECT

public:
  WorkspaceModel();

public:
  QStandardItem * geometryItem();
  void setCurrentParent(QStandardItem * item);
  void setProject(Project * project);

private slots:
  void handleAddItem(ProjectItem * projectItem);

private:
  void createDefaultItems();
  void loadProject();

private:
  Project * mProject;
  QStandardItem * mGeometryItem;
  QStandardItem * mMeshItem;
  QStandardItem * mExcitationsItems;
  QStandardItem * mObservationsItem;
  QStandardItem * mCurrentParent;
};

#endif // WORKSPACEMODEL_H
