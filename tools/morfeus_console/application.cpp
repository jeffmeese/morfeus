#include "application.h"

#include "commandline.h"
#include "geometry.h"
#include "inputdata.h"
#include "materialdatabase.h"
#include "mesh.h"
#include "mesher.h"
#include "solution.h"

Application::Application(int argc, char ** argv)
  : mCommandLine(new CommandLine)
  , mInputData(new InputData)
  , mMesh(new Mesh)
{
  parseCommandLine(argc, argv);
}

Application::~Application()
{
}

void Application::createMesh()
{
  std::cout << "Creating Mesh...";
  const Mesher * mesher = mInputData->mesher();

  // Create the mesh
  mMesh->setMaterialDatabase(mInputData->materialDatabase());
  mesher->createMesh(mInputData->geometry(), mInputData->cavityHeight(), mMesh.get());
  mMesh->writeGeomFile("test.geom");
  std::cout << "done\n";
}

void Application::execute()
{
  readInput();
  createMesh();
  //runSolution();
}

void Application::parseCommandLine(int argc, char **argv)
{
  mFileName = argv[argc-1];
}

void Application::readInput()
{
  try {
    mInputData->readFromFile(mFileName);
    mInputData->validate();
    mInputData->print();
    mInputData->saveToFile("temp.xml");
  }
  catch (std::invalid_argument & ex) {
    std::cout << ex.what() << "\n";
    exit(1);
  }
}

void Application::runSolution()
{
  Solution * solution = mInputData->solution();
  solution->runSolution(mMesh.get());
}
