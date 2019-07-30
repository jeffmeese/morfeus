#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include <string>

class CommandLine;
namespace morfeus {
class MeshInformation;
class MorfeusProject;
namespace mesh {
  class Mesh;
}
namespace observation {
class Observation;
}
}

class Application
{
public:
  Application(int argc, char ** argv);
  ~Application();

public:
  void execute();

private:
  void createMesh(morfeus::mesh::Mesh * mesh);
  void parseCommandLine(int argc, char ** argv);
  void readInput();
  void reportObservation(const morfeus::observation::Observation * observation);
  void runSolution(const morfeus::mesh::Mesh * mesh, const morfeus::MeshInformation * meshInfo);

private:
  std::string mFileName;
  std::unique_ptr<CommandLine> mCommandLine;
  std::unique_ptr<morfeus::MorfeusProject> mProject;
};

#endif // APPLICATION_H
