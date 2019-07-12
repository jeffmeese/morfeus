#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Application;
namespace Ui {
  class MainWindow;
}

class MainWindow
    : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(Application & application, QWidget *parent = nullptr);
  ~MainWindow();

protected:
  void closeEvent(QCloseEvent * closeEvent) override;

private slots:
  void handleExit();
  void handleNewProject();
  void handleOpenProject();
  void handleSaveProject();
  void handleSaveProjectAs();
  void handleSettings();

private:
  void exitProgram();
  void initActions();
  void newProject();
  void openProject(const QString & fileName);
  bool promptSave();
  void saveProject(const QString & fileName);

private:
  std::unique_ptr<Ui::MainWindow> mUi;
  Application & mApplication;
};

#endif // MAINWINDOW_H
