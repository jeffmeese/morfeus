#include "application.h"

#include "commandline.h"
#include "geometry.h"
#include "inputdata.h"
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
  const Mesher * mesher = mInputData->mesher();

  // Create the mesh
  mMesh->setMaterialDatabase(mInputData->materialDatabase());
  mesher->createMesh(mInputData->geometry(), mInputData->cavityHeight(), mMesh.get());
}

void Application::execute()
{
  readInput();
  createMesh();
  runSolution();
}

void Application::parseCommandLine(int argc, char **argv)
{
  mFileName = argv[argc-1];
}

void Application::readInput()
{
  mInputData->readFromFile(mFileName);
  mInputData->validate();
}

void Application::runSolution()
{
  Solution * solution = mInputData->solution();

  double freqStart = mInputData->freqStart();
  double freqStop = mInputData->freqStop();
  double freqIncr = mInputData->freqIncrement();
  std::size_t totalFrequencies = static_cast<std::size_t>( (freqStop - freqStart) / freqIncr);
  for (std::size_t i = 0; i < totalFrequencies; i++) {
    double freq = freqStart + i*freqIncr;
    solution->runSolution(freq, mMesh.get());
  }
}
