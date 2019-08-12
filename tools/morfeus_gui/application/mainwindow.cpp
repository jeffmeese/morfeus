#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "application.h"
#include "addrectanglecommand.h"
#include "cubedialog.h"
#include "guiproject.h"
#include "partitem.h"
#include "view3d.h"
#include "workspacemodel.h"

#include <model/geometry/cuboid.h>
#include <model/geometry/cylinder.h>
#include <model/geometry/rectangle.h>

#include <action.h>
#include <actionmanager.h>

#include <QCloseEvent>
#include <QAbstractButton>

MainWindow::MainWindow(Application & application, QWidget *parent)
  : QMainWindow(parent)
  , mApplication(application)
  , mUi(new Ui::MainWindow)
  , mWorkspaceModel(new WorkspaceModel)
{
  initialize();
  newProject();
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

void MainWindow::create3dView()
{
  View3d * view = new View3d(this);
  QObject::connect(mWorkspaceModel.get(), &WorkspaceModel::partAdded, view, &View3d::handleAddPart);
  mUi->cMainTab->addTab(view, "View3d");
}

void MainWindow::exitProgram()
{
  saveSettings();
  QApplication::exit();
}

void MainWindow::handleAddCube()
{
  using namespace morfeus::model::geometry;
  qDebug() << QObject::tr(__FUNCTION__);

  QString name = "Cube" + QString::number(Cuboid::totalCreated()+1);
  CubeDialog dialog(this);
  dialog.setX(0.0);
  dialog.setY(0.0);
  dialog.setZ(0.0);
  dialog.setWidth(1.0);
  dialog.setHeight(1.0);
  dialog.setLength(1.0);
  dialog.setName(name);

  int response = dialog.exec();
  if (response == QDialog::Rejected)
    return;

  std::unique_ptr<Cuboid> cube(new Cuboid);
  cube->setName(dialog.name().toStdString());
  cube->setBaseCenter(dialog.x(), dialog.y(), dialog.z());
  cube->setWidth(dialog.width());
  cube->setHeight(dialog.height());
  cube->setLength(dialog.length());

  mWorkspaceModel->addPart(std::move(cube));
  updateWindowTitle();
}

void MainWindow::handleAddCylinder()
{
  using namespace morfeus::model::geometry;
  std::unique_ptr<Cylinder> cylinder(new Cylinder);
  cylinder->setRadius(1.0);
  cylinder->setHeight(1.0);

  mWorkspaceModel->addPart(std::move(cylinder));
  updateWindowTitle();
}

void MainWindow::handleAddEllipse()
{

}

void MainWindow::handleAddPolygon()
{

}

void MainWindow::handleAddRectangle()
{
  qDebug() << QObject::tr(__FUNCTION__);

  using namespace morfeus::model;
  std::unique_ptr<geometry::Rectangle> rectangle(new geometry::Rectangle);
  mWorkspaceModel->addPart(std::move(rectangle));
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

  newProject();
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

void MainWindow::handleWorkspaceSelection(const QModelIndex &current, const QModelIndex & preious)
{
  qDebug() << QObject::tr(__FUNCTION__);

  QStandardItem * item = mWorkspaceModel->itemFromIndex(current);
  PartItem * partItem = dynamic_cast<PartItem*>(item);
  if (partItem != nullptr) {
    mUi->cDetailsView->displayPartProperties(partItem->part());
  }
}

void MainWindow::handleUndo()
{
  mApplication.commandStack().undo();
  updateWindowTitle();
}

void MainWindow::initialize()
{
  initUi();
  initActions();
  loadSettings();
}

void MainWindow::initActions()
{
  // Program actions
//  connect(mUi->actionExit, SIGNAL(triggered()), SLOT(handleExit()));
//  connect(mUi->actionNew_Project, SIGNAL(triggered()), SLOT(handleNewProject()));
//  connect(mUi->actionOpen_Project, SIGNAL(triggered()), SLOT(handleOpenProject()));
//  connect(mUi->actionSave_Project, SIGNAL(triggered()), SLOT(handleSaveProject()));
//  connect(mUi->actionSave_Project_As, SIGNAL(triggered()), SLOT(handleSaveProjectAs()));
//  connect(mUi->actionSettings, SIGNAL(triggered()), SLOT(handleSettings()));

//  // Project actions
//  connect(mUi->actionAdd_Rectangle, SIGNAL(triggered()), SLOT(handleAddRectangle()));

//  // Tools actions
//  //connect(mUi->menuTools, SIGNAL(aboutToShow()), SLOT(updateToolsMenu()));
//  connect(mUi->actionRedo, SIGNAL(triggered()), SLOT(handleRedo()));
//  connect(mUi->actionUndo, SIGNAL(triggered()), SLOT(handleUndo()));
}

void MainWindow::createToolButton(const QString & tabName, const QString & groupName, const QString & text,
                                  const QString & toopTip, const QIcon & icon, void (MainWindow::*receiver)())
{
  QToolButton * newButton = new QToolButton;
  newButton->setText(text);
  newButton->setToolTip(toopTip);
  newButton->setIcon(icon);
  mUi->cRibbon->addButton(tabName, groupName, newButton);
  QObject::connect(newButton, &QAbstractButton::clicked, this, receiver);
}

void MainWindow::initRibbon()
{
  mUi->cRibbonDocker->setTitleBarWidget(new QWidget);

  mUi->cRibbon->addTab("Project");
  createToolButton("Project", "Project", "New", "Create new project", style()->standardIcon(QStyle::SP_FileIcon), &MainWindow::handleNewProject);
  createToolButton("Project", "Project", "Open", "Open an existing project", style()->standardIcon(QStyle::SP_DirIcon), &MainWindow::handleOpenProject);
  createToolButton("Project", "Project", "Save", "Save the current project", style()->standardIcon(QStyle::SP_DriveFDIcon), &MainWindow::handleSaveProject);

  mUi->cRibbon->addTab("Model");
  createToolButton("Model", "Solids", "Cube", "Construct a cube", QIcon(":/images/icons/cube.png"), &MainWindow::handleAddCube);
  createToolButton("Model", "Solids", "Cylinder", "Construct a cylinder", QIcon(":/images/icons/cylinder.png"), &MainWindow::handleAddCylinder);
  createToolButton("Model", "Surfaces", "Rectangle", "Construct a rectangle", QIcon(":/images/icons/square.png"), &MainWindow::handleAddRectangle);
  createToolButton("Model", "Surfaces", "Polygon", "Construct a polygon", QIcon(":/images/icons/hexagon.png"), &MainWindow::handleAddPolygon);
  createToolButton("Model", "Surfaces", "Ellipse", "Construct an ellipse", QIcon(":/images/icons/circle.png"), &MainWindow::handleAddEllipse);

  mUi->cRibbon->addTab("Transform");
  mUi->cRibbon->addTab("Sources");
  mUi->cRibbon->addTab("Mesh");
  mUi->cRibbon->addTab("Observations");
  mUi->cRibbon->addTab("Solver");
  mUi->cRibbon->addTab("Tools");
  mUi->cRibbon->addTab("View");
  mUi->cRibbon->addTab("Display");
}

void MainWindow::initToolBar()
{
  QAction * newAction = mUi->toolbar->addAction(style()->standardIcon(QStyle::SP_FileIcon), "New");
  QAction * openAction = mUi->toolbar->addAction(style()->standardIcon(QStyle::SP_DirIcon), "Open");
  QAction * saveAction = mUi->toolbar->addAction(style()->standardIcon(QStyle::SP_DriveFDIcon), "Save");
  mUi->toolbar->addSeparator();
  QAction * settingsAction = mUi->toolbar->addAction(QIcon(":/images/icons/settings.png"), "Settings");
  mUi->toolbar->addSeparator();
  QAction * undoAction = mUi->toolbar->addAction(QIcon(":/images/icons/undo.png"), "Undo");
  QAction * redoAction = mUi->toolbar->addAction(QIcon(":/images/icons/redo.png"), "Redo");
}

void MainWindow::initUi()
{
  mUi->setupUi(this);
  initRibbon();
  initToolBar();
}

void MainWindow::loadSettings()
{

}

void MainWindow::newProject()
{
  mApplication.newProject();
  mApplication.commandStack().clear();
  mUi->cMainTab->clear();
  mUi->cDetailsView->setModel(nullptr);
  resetModels();
  create3dView();
  updateWindowTitle();
}

void MainWindow::openProject(const QString & fileName)
{
  mApplication.project()->load(fileName);
  mApplication.commandStack().clear();
  mUi->cMainTab->clear();
  resetModels();
  create3dView();
  updateWindowTitle();
}

bool MainWindow::promptSave()
{
  return true;
}

void MainWindow::resetModels()
{
  mWorkspaceModel.reset(new WorkspaceModel);
  mUi->cWorkspaceView->setModel(mWorkspaceModel.get());

  QObject::connect(mUi->cWorkspaceView->selectionModel(), &QItemSelectionModel::currentChanged,
                   this, &MainWindow::handleWorkspaceSelection);
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
