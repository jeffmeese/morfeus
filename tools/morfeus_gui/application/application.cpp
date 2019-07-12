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

void Application::newProject()
{
  mProject.reset(new Project);
}
