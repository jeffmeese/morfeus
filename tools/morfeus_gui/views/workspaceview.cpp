#include "workspaceview.h"

#include "addrectanglecommand.h"
#include "application.h"
#include "guiproject.h"
#include "renamecommand.h"
#include "workspacemodel.h"
#include "workspacemodelitem.h"

#include <model/geometry/rectangle.h>

#include <action.h>
#include <actionmanager.h>

#include <QDebug>
#include <QInputDialog>
#include <QMenu>
#include <QModelIndex>

WorkspaceView::WorkspaceView(QWidget * parentWidget)
  : QTreeView(parentWidget)
  , mProject(nullptr)
{
  initActions();
  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(handleContextMenu(QPoint)));
}


ActionManager & WorkspaceView::actionManager()
{
  static ActionManager manager;
  return manager;
}

void WorkspaceView::handleAddRectangle()
{
//  WorkspaceModel * workspaceModel = dynamic_cast<WorkspaceModel*>(model());
//  if (workspaceModel == nullptr) {
//    return;
//  }

//  QList<QModelIndex> indexList = selectionModel()->selectedIndexes();
//  if (indexList.size() != 1)
//    return;

//  QModelIndex index = indexList.at(0);
//  WorkspaceModelItem * modelItem = dynamic_cast<WorkspaceModelItem*>(workspaceModel->itemFromIndex(index));

//  std::unique_ptr<morfeus::geometry::Rectangle> rectangle(new morfeus::geometry::Rectangle);
//  AddRectangleCommand * command = new AddRectangleCommand(mProject, std::move(rectangle));
//  Application::commandStack().push(command);
//  mProject->setModified(true);
}

void WorkspaceView::handleContextMenu(const QPoint &menuPos)
{
  qDebug() << QObject::tr(__FUNCTION__);

  if (model() == nullptr)
    return;

  WorkspaceModel * workspaceModel = dynamic_cast<WorkspaceModel*>(model());
  if (workspaceModel == nullptr)
    return;

  QModelIndexList indexList = selectionModel()->selectedRows();
  if (indexList.size() == 0)
    return;

  QList<Action*> actionList;
  for (int i = 0; i < indexList.size(); i++) {
    QModelIndex modelIndex = indexList.at(i);
    QStandardItem * standardItem = workspaceModel->itemFromIndex(modelIndex);
    WorkspaceModelItem * modelItem = dynamic_cast<WorkspaceModelItem*>(standardItem);
    if (modelItem != nullptr) {
      actionList = modelItem->getActionList();
    }
  }

  if (actionList.size() > 0) {
    QMenu menu(this);
    for (int i = 0; i < actionList.size(); i++) {
      menu.addAction(actionList.at(i));
    }

    menu.exec(mapToGlobal(menuPos));
  }
}

void WorkspaceView::handleRename()
{
  if (model() == nullptr)
    return;

  QStandardItemModel * itemModel = dynamic_cast<QStandardItemModel*>(model());
  if (itemModel == nullptr)
    return;

  QModelIndexList indexList = selectionModel()->selectedRows();
  if (indexList.size() != 1)
    return;

  QModelIndex index = indexList.at(0);
  QStandardItem * item = itemModel->itemFromIndex(index);
  WorkspaceModelItem * modelItem = dynamic_cast<WorkspaceModelItem*>(item);
  if (modelItem != nullptr) {

    QString title("Rename");
    QString label("Enter new name");
    QString text(modelItem->text());
    QString newName = QInputDialog::getText(this, title, label, QLineEdit::Normal, text);
    if (!newName.isEmpty()) {
      RenameCommand * command = new RenameCommand(newName);
      Application::commandStack().push(command);
    }
  }
}

void WorkspaceView::initActions()
{
  Action * addRectangleCommand = actionManager().registerAction(AddRectangleCommand::commandId());
  addRectangleCommand->setText("Add Rectangle");
  connect(addRectangleCommand, SIGNAL(triggered()), SLOT(handleAddRectangle()));

  Action * renameCommand = actionManager().registerAction(RenameCommand::commandId());
  renameCommand->setText("Rename");
  connect(renameCommand, SIGNAL(triggered()), SLOT(handleRename()));
}

void WorkspaceView::setProject(GuiProject *project)
{
  mProject = project;
}
