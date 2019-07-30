#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "application.h"
#include "addrectanglecommand.h"
#include "guiproject.h"
#include "rectangle.h"
#include "workspacemodel.h"

#include <action.h>
#include <actionmanager.h>

#include <QCloseEvent>

MainWindow::MainWindow(Application & application, QWidget *parent)
  : QMainWindow(parent)
  , mApplication(application)
  , mUi(new Ui::MainWindow)
  , mWorkspaceModel(new WorkspaceModel)
{
  initUi();
  initActions();
  loadSettings();
  resetModels();
  updateWindowTitle();
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *closeEvent)
{
  if (!promptSave()) {
    closeEvent->ignore();
    return;
  }

  closeEvent->accept();
}

void MainWindow::exitProgram()
{
  saveSettings();
  QApplication::exit();
}

void MainWindow::handleAddRectangle()
{
  GuiProject * project = mApplication.project();
  std::unique_ptr<morfeus::geometry::Rectangle> rectangle(new morfeus::geometry::Rectangle);
  AddRectangleCommand * command = new AddRectangleCommand(project, std::move(rectangle));
  mApplication.commandStack().push(command);
  updateWindowTitle();
}

void MainWindow::handleCreateMesh()
{

}

void MainWindow::handleExit()
{
  if (!promptSave())
    return;

  exitProgram();
}

void MainWindow::handleNewProject()
{
  if (!promptSave())
    return;
}

void MainWindow::handleOpenProject()
{
  if (!promptSave())
    return;
}

void MainWindow::handleProjectModified()
{
  updateWindowTitle();
}

void MainWindow::handleRedo()
{
  mApplication.commandStack().redo();
  updateWindowTitle();
}

void MainWindow::handleSaveProject()
{
  QString fileName = mApplication.project()->fileName();
  if (fileName.isEmpty()) {
    handleSaveProjectAs();
    return;
  }
  saveProject(fileName);
}

void MainWindow::handleSaveProjectAs()
{

}

void MainWindow::handleSettings()
{
}

void MainWindow::handleUndo()
{
  mApplication.commandStack().undo();
  updateWindowTitle();
}

void MainWindow::initActions()
{
  // Program actions
  connect(mUi->actionExit, SIGNAL(triggered()), SLOT(handleExit()));
  connect(mUi->actionNew_Project, SIGNAL(triggered()), SLOT(handleNewProject()));
  connect(mUi->actionOpen_Project, SIGNAL(triggered()), SLOT(handleOpenProject()));
  connect(mUi->actionSave_Project, SIGNAL(triggered()), SLOT(handleSaveProject()));
  connect(mUi->actionSave_Project_As, SIGNAL(triggered()), SLOT(handleSaveProjectAs()));
  connect(mUi->actionSettings, SIGNAL(triggered()), SLOT(handleSettings()));

  // Project actions
  connect(mUi->actionAdd_Rectangle, SIGNAL(triggered()), SLOT(handleAddRectangle()));

  // Tools actions
  connect(mUi->menuTools, SIGNAL(aboutToShow()), SLOT(updateToolsMenu()));
  connect(mUi->actionRedo, SIGNAL(triggered()), SLOT(handleRedo()));
  connect(mUi->actionUndo, SIGNAL(triggered()), SLOT(handleUndo()));
}

void MainWindow::initUi()
{
  mUi->setupUi(this);
}

void MainWindow::loadSettings()
{

}

void MainWindow::newProject()
{
  mApplication.newProject();
  mApplication.commandStack().clear();
  resetModels();
  updateWindowTitle();
}

void MainWindow::openProject(const QString & fileName)
{
  mApplication.project()->load(fileName);
  mApplication.commandStack().clear();
  resetModels();
  updateWindowTitle();
}

bool MainWindow::promptSave()
{
  return true;
}

void MainWindow::resetModels()
{
  GuiProject * project = mApplication.project();
  mWorkspaceModel->setProject(project);
  mUi->cWorkspaceView->setProject(project);
  mUi->cWorkspaceView->setModel(mWorkspaceModel.get());
  connect(project, SIGNAL(modified(bool)), SLOT(handleProjectModified()));
}

void MainWindow::saveProject(const QString & fileName)
{
  mApplication.project()->save(fileName);
  updateWindowTitle();
}

void MainWindow::saveSettings()
{
}

void MainWindow::updateToolsMenu()
{
  mUi->actionRedo->setEnabled(mApplication.commandStack().canRedo());
  mUi->actionUndo->setEnabled(mApplication.commandStack().canUndo());
}

void MainWindow::updateWindowTitle()
{
  QString fileName = mApplication.project()->fileName();
  bool modified = mApplication.project()->modified();

  QString windowTitle("Morfeus GUI - ");
  if (fileName.isEmpty()) {
    fileName = "Untitled";
  }
  windowTitle += fileName;

  if (modified) {
    windowTitle += "*";
  }

  setWindowTitle(windowTitle);
}
