#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include <string>

class CommandLine;
namespace Morfeus {
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
  void createMesh(Morfeus::mesh::Mesh * mesh);
  void parseCommandLine(int argc, char ** argv);
  void readInput();
  void reportObservation(const Morfeus::observation::Observation * observation);
  void runSolution(const Morfeus::mesh::Mesh * mesh, const Morfeus::MeshInformation * meshInfo);

private:
  std::string mFileName;
  std::unique_ptr<CommandLine> mCommandLine;
  std::unique_ptr<Morfeus::MorfeusProject> mProject;
};

#endif // APPLICATION_H
