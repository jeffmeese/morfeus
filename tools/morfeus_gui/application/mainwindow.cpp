#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "workspacemodel.h"

#include <QCloseEvent>

MainWindow::MainWindow(Application & application, QWidget *parent)
  : QMainWindow(parent),
    mUi(new Ui::MainWindow)
  , mApplication(application)
{
  mUi->setupUi(this);
  mUi->cWorkspaceView->setModel(new WorkspaceModel);
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

void MainWindow::handleSaveProject()
{
}

void MainWindow::handleSaveProjectAs()
{

}

void MainWindow::handleSettings()
{
}

void MainWindow::initActions()
{
  connect(mUi->actionExit, SIGNAL(triggered()), SLOT(handleExit()));
  connect(mUi->actionNew_Project, SIGNAL(triggered()), SLOT(handleNewProject()));
  connect(mUi->actionOpen_Project, SIGNAL(triggered()), SLOT(handleOpenProject()));
  connect(mUi->actionSave_Project, SIGNAL(triggered()), SLOT(handleSaveProject()));
  connect(mUi->actionSave_Project_As, SIGNAL(triggered()), SLOT(handleSaveProjectAs()));
  connect(mUi->actionSettings, SIGNAL(triggered()), SLOT(handleSettings()));
}

void MainWindow::newProject()
{

}

void MainWindow::openProject(const QString & fileName)
{

}

bool MainWindow::promptSave()
{
  return true;
}

void MainWindow::saveProject(const QString & fileName)
{

}
