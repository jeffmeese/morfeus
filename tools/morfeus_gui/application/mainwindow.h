#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
  void handleAddRectangle();
  void handleCreateMesh();
  void handleExit();
  void handleNewProject();
  void handleOpenProject();
  void handleRedo();
  void handleSaveProject();
  void handleSaveProjectAs();
  void handleSettings();
  void handleUndo();
  void updateToolsMenu();

private:
  void exitProgram();
  void initActions();
  void initUi();
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
