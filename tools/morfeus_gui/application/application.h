#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QUndoStack>

#include <memory>

class GuiProject;

class Application
    : public QApplication
{
public:
  Application(int & argc, char ** argv);
  ~Application();

public:
  const GuiProject * project() const;
  GuiProject * project();

public:
  void newProject();

public:
  static QUndoStack & commandStack();

private:
  std::unique_ptr<GuiProject> mProject;
};

inline const GuiProject * Application::project() const
{
  return mProject.get();
}

inline GuiProject * Application::project()
{
  return mProject.get();
}

#endif // APPLICATION_H
