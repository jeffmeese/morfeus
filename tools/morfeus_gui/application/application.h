#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

class Project;

class Application
    : public QApplication
{
public:
  Application(int & argc, char ** argv);
  ~Application();

public:
  void newProject();

private:
  std::unique_ptr<Project> mProject;
};

#endif // APPLICATION_H
