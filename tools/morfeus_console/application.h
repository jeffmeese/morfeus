#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include <string>

class CommandLine;
class Geometry;
class InputData;
class Mesh;

class Application
{
public:
  Application(int argc, char ** argv);
  ~Application();

public:
  void execute();

private:
  void buildGeometry();
  void createMesh();
  void parseCommandLine(int argc, char ** argv);
  void readInput();
  void runSolution();

private:
  std::string mFileName;
  std::unique_ptr<CommandLine> mCommandLine;
  std::unique_ptr<Geometry> mGeometry;
  std::unique_ptr<InputData> mInputData;
  std::unique_ptr<Mesh> mMesh;
};

#endif // APPLICATION_H
