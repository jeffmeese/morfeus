#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QUndoStack>

#include <memory>

class Project;

class Application
    : public QApplication
{
public:
  Application(int & argc, char ** argv);
  ~Application();

public:
  const Project * project() const;
  Project * project();

public:
  void newProject();

public:
  static QUndoStack & commandStack();

private:
  std::unique_ptr<Project> mProject;
};

inline const Project * Application::project() const
{
  return mProject.get();
}

inline Project * Application::project()
{
  return mProject.get();
}

#endif // APPLICATION_H
