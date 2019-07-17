#include "application.h"

#include "commandline.h"
#include "inputdata.h"

Application::Application(int argc, char ** argv)
  : mCommandLine(new CommandLine)
  , mInputData(new InputData)
{
  parseCommandLine(argc, argv);
}

void Application::execute()
{

}

void Application::parseCommandLine(int argc, char **argv)
{

}
