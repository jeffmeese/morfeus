#include "application.h"

#include <project.h>

Application::Application(int & argc, char ** argv)
  : QApplication (argc, argv)
{
  newProject();
}

Application::~Application()
{

}

QUndoStack & Application::commandStack()
{
  static QUndoStack commandStack;
  return commandStack;
}

void Application::newProject()
{
  mProject.reset(new Project);
}
