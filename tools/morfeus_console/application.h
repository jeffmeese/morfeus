#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>

class CommandLine;
class InputData;

class Application
{
public:
  Application(int argc, char ** argv);

public:
  void execute();

private:
  void parseCommandLine(int argc, char ** argv);

private:
  std::unique_ptr<InputData> mInputData;
  std::unique_ptr<CommandLine> mCommandLine;
};

#endif // APPLICATION_H
