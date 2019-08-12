#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>

#include <memory>

class Application;
class WorkspaceModel;
namespace Ui {
  class MainWindow;
}

class MainWindow
    : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(Application & application, QWidget *parent = nullptr);
  ~MainWindow() override;

protected:
  void closeEvent(QCloseEvent * closeEvent) override;

private slots:
  void handleAddCube();
  void handleAddCylinder();
  void handleAddEllipse();
  void handleAddPolygon();
  void handleAddRectangle();
  void handleCreateMesh();
  void handleExit();
  void handleNewProject();
  void handleOpenProject();
  void handleProjectModified();
  void handleRedo();
  void handleSaveProject();
  void handleSaveProjectAs();
  void handleSettings();
  void handleUndo();
  void handleWorkspaceSelection(const QModelIndex & current, const QModelIndex & previous);
  void updateToolsMenu();

private:
  void createToolButton(const QString & tabName, const QString & groupName, const QString & name,
                        const QString & toolTip, const QIcon & icon, void (MainWindow::*recevier)());
  void create3dView();
  void exitProgram();
  void initialize();
  void initActions();
  void initUi();
  void initRibbon();
  void initToolBar();
  void loadSettings();
  void newProject();
  void openProject(const QString & fileName);
  bool promptSave();
  void resetModels();
  void saveProject(const QString & fileName);
  void saveSettings();
  void updateWindowTitle();

private:
  Application & mApplication;
  std::unique_ptr<Ui::MainWindow> mUi;
  std::unique_ptr<WorkspaceModel> mWorkspaceModel;
};

#endif // MAINWINDOW_H
